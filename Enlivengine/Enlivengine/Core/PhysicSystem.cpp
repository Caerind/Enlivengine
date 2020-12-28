#include <Enlivengine/Core/PhysicSystem.hpp>

#include <Enlivengine/Utils/Profiler.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/PhysicComponent.hpp>

namespace en
{

PhysicSystem::PhysicSystem()
	: System()
	, mPhysicWorld(nullptr)
	, mVelocityIterations(8)
	, mPositionIterations(3)
#if defined(ENLIVE_DEBUG)
	, mDebugRenderFlags(static_cast<U32>(b2Draw::e_shapeBit))
	, mDebugRender(false)
#endif // ENLIVE_DEBUG
{
	mPhysicWorld = new b2World(b2Vec2(0.0f, 0.0f));
	enAssert(mPhysicWorld != nullptr);

#if defined(ENLIVE_DEBUG)
	mPhysicWorld->SetDebugDraw(static_cast<b2Draw*>(this));
	SetFlags(mDebugRenderFlags);
#endif // ENLIVE_DEBUG

	mPhysicWorld->SetContactListener(static_cast<b2ContactListener*>(this));
}

PhysicSystem::~PhysicSystem()
{
	delete mPhysicWorld;
}

bool PhysicSystem::Initialize(const Entity& entity, PhysicComponent& component)
{
	enAssert(entity.IsValid());

	static b2BodyDef bodyDef;
	if (entity.Has<TransformComponent>())
	{
		const TransformComponent& transform = entity.Get<TransformComponent>();
		bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y);
		// TODO : Fix this
		//bodyDef.angle = Math::DegToRad(transform.GetRotation2D());
	}
	else
	{
		bodyDef.position.Set(0.0f, 0.0f);
		bodyDef.angle = 0.0f;
	}

	component.mEntity = entity;
	component.mBody = mPhysicWorld->CreateBody(&bodyDef);
	if (component.mBody != nullptr)
	{
		component.mBody->SetUserData(&component);
	}

	return component.IsValid();
}

bool PhysicSystem::Deinitialize(const Entity& entity, PhysicComponent& component)
{
	if (!entity.IsValid())
	{
		return false;
	}

	if (component.mBody != nullptr)
	{
		// Unregister BeginContact/EndContact signals
		auto itr = mContactSignals.find(component.mBody);
		if (itr != mContactSignals.end())
		{
			mContactSignals.erase(itr);
		}

		if (mPhysicWorld != nullptr)
		{
			mPhysicWorld->DestroyBody(component.mBody);
		}
	}

	return true;
}

void PhysicSystem::Update(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	BeforeUpdate();
	{
		ENLIVE_PROFILE_SCOPE("Box2D_Update");
		mPhysicWorld->Step(dt.AsSeconds(), mVelocityIterations, mPositionIterations);
	}
	AfterUpdate();
}

void PhysicSystem::BeforeUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
	auto& entityManager = mWorld->GetEntityManager();
	auto view = entityManager.View<TransformComponent, PhysicComponent>();
	for (auto entt : view)
	{
		Entity entity(entityManager, entt);
		if (entity.IsValid())
		{
			auto& physicComponent = entity.Get<PhysicComponent>();
			if (auto* body = physicComponent.GetBody())
			{
				const auto& transformComponent = entity.Get<TransformComponent>();
				const auto position = transformComponent.GetPosition();
				// TODO : Fix this
				body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
			}
		}
	}
}

void PhysicSystem::AfterUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
	auto& entityManager = mWorld->GetEntityManager();
	auto view = entityManager.View<TransformComponent, PhysicComponent>();
	for (auto entt : view)
	{
		en::Entity entity(entityManager, entt);
		if (entity.IsValid())
		{
			const auto& physicComponent = entity.Get<en::PhysicComponent>();
			if (physicComponent.GetBodyType() != en::PhysicBodyType::Static)
			{
				if (const auto* body = physicComponent.GetBody())
				{
					const auto& position = body->GetPosition();
					auto& transformComponent = entity.Get<en::TransformComponent>();
					// TODO : Fix this
					transformComponent.SetPosition(Vector3f(position.x, position.y, transformComponent.GetPosition().z));
					//transformComponent.SetRotation(en::Math::RadToDeg(body->GetAngle()));
				}
			}
		}
	}
}

void PhysicSystem::DisableGravity()
{
	SetGravity(Vector2f(0.0f, 0.0f));
}

void PhysicSystem::SetGravity(const Vector2f& gravity)
{
	mPhysicWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

Vector2f PhysicSystem::GetGravity() const
{
	const b2Vec2 gravity = mPhysicWorld->GetGravity();
	return Vector2f(gravity.x, gravity.y);
}

void PhysicSystem::SetVelocityIterations(U32 value)
{
	mVelocityIterations = value;
}

U32 PhysicSystem::GetVelocityIterations() const
{
	return mVelocityIterations;
}

void PhysicSystem::SetPositionIterations(U32 value)
{
	mPositionIterations = value;
}

U32 PhysicSystem::GetPositionIterations() const
{
	return mPositionIterations;
}

void PhysicSystem::BeginContact(b2Contact* contact)
{
	if (contact != nullptr)
	{
		b2Fixture* fA = contact->GetFixtureA();
		b2Fixture* fB = contact->GetFixtureB();

		if (fA != nullptr)
		{
			if (b2Body* bA = fA->GetBody())
			{
				mContactSignals[bA].BeginContact(contact, fA, fB);
			}
		}

		if (fB != nullptr)
		{
			if (b2Body* bB = fB->GetBody())
			{
				mContactSignals[bB].BeginContact(contact, fB, fA);
			}
		}
	}
}

void PhysicSystem::EndContact(b2Contact* contact)
{
	if (contact != nullptr)
	{
		b2Fixture* fA = contact->GetFixtureA();
		b2Fixture* fB = contact->GetFixtureB();

		if (fA != nullptr)
		{
			if (b2Body* bA = fA->GetBody())
			{
				mContactSignals[bA].EndContact(contact, fA, fB);
			}
		}

		if (fB != nullptr)
		{
			if (b2Body* bB = fB->GetBody())
			{
				mContactSignals[bB].EndContact(contact, fB, fA);
			}
		}
	}
}

#if defined(ENLIVE_DEBUG)
void PhysicSystem::Render()
{
	ENLIVE_PROFILE_FUNCTION();
	if (mDebugRender)
	{
		mPhysicWorld->DrawDebugData();
	}
}

void PhysicSystem::SetDebugRendering(bool value)
{
	mDebugRender = value;
}

bool PhysicSystem::IsDebugRendering() const
{
	return mDebugRender;
}

void PhysicSystem::SetDebugRenderFlags(U32 flags)
{
	mDebugRenderFlags = flags;
	SetFlags(mDebugRenderFlags);
}

U32 PhysicSystem::GetDebugRenderFlags() const
{
	return mDebugRenderFlags;
}

void PhysicSystem::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	Color c;
	c.FromBox2DColor(color);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		const Vector3f pos1(vertices[i].x, vertices[i].y, 0.0f);
		Vector3f pos2;
		if (i < vertexCount - 1)
		{
			pos2.Set(vertices[i+1].x, vertices[i+1].y, 0.0f);
		}
		else
		{
			pos2.Set(vertices[0].x, vertices[0].y, 0.0f);
		}
		mWorld->GetDebugDraw().DrawLine(pos1, pos2, c);
	}
}

void PhysicSystem::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	// TODO : Fill draw
	DrawPolygon(vertices, vertexCount, color);
}

void PhysicSystem::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	ENLIVE_UNUSED(center);
	ENLIVE_UNUSED(radius);
	ENLIVE_UNUSED(color);
	// TODO : Debug draw circle
	/*
	sf::CircleShape circle(radius * mPixelsPerMeter);
	circle.setOrigin(radius * mPixelsPerMeter, radius * mPixelsPerMeter);
	circle.setPosition(sf::Vector2f(center.x, center.y) * mPixelsPerMeter);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255)));
	circle.setOutlineThickness(-1.0f);

	if (mDebugRenderTarget != nullptr)
	{
		mDebugRenderTarget->draw(circle);
	}
	*/
}

void PhysicSystem::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	ENLIVE_UNUSED(axis);

	// TODO : Fill draw
	DrawCircle(center, radius, color);
}

void PhysicSystem::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	const Vector3f pos1(p1.x, p1.y, 0.0f);
	const Vector3f pos2(p2.x, p2.y, 0.0f);
	Color c;
	c.FromBox2DColor(color);
	mWorld->GetDebugDraw().DrawLine(pos1, pos2, c);
}

void PhysicSystem::DrawTransform(const b2Transform& xf)
{
	constexpr F32 lineLength = 0.4f;
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetXAxis(), Colors::Red.ToBox2DColor());
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetYAxis(), Colors::Green.ToBox2DColor());
}

void PhysicSystem::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	ENLIVE_UNUSED(size);

	const Vector3f pos(p.x, p.y, 0.0f);
	Color c;
	c.FromBox2DColor(color);
	mWorld->GetDebugDraw().DrawPoint(pos, c);
}

b2Body* PhysicSystem::GetComponentBody(const PhysicComponent& component)
{
	return const_cast<b2Body*>(component.GetBody());
}

#endif // ENLIVE_DEBUG

} // namespace en