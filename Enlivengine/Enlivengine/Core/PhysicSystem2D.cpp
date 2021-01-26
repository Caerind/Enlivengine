#include <Enlivengine/Core/PhysicSystem2D.hpp>

#include <Enlivengine/Utils/Profiler.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/PhysicComponent2D.hpp>

namespace en
{

PhysicSystem2D::PhysicSystem2D()
	: PhysicSystemBase()
	, mPhysicWorld(nullptr)
	, mInternalSystem(*this)
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
	mPhysicWorld->SetDebugDraw(static_cast<b2Draw*>(&mInternalSystem));
	mInternalSystem.SetFlags(mDebugRenderFlags);
#endif // ENLIVE_DEBUG

	mPhysicWorld->SetContactListener(static_cast<b2ContactListener*>(&mInternalSystem));
}

PhysicSystem2D::~PhysicSystem2D()
{
	delete mPhysicWorld;
}

bool PhysicSystem2D::Initialize(const Entity& entity, PhysicComponent2D& component)
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

bool PhysicSystem2D::Deinitialize(const Entity& entity, PhysicComponent2D& component)
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

void PhysicSystem2D::Update(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	BeforeUpdate();
	{
		ENLIVE_PROFILE_SCOPE("Box2D_Update");
		mPhysicWorld->Step(dt.AsSeconds(), mVelocityIterations, mPositionIterations);
	}
	AfterUpdate();
}

void PhysicSystem2D::BeforeUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
	auto& entityManager = mWorld->GetEntityManager();
	auto view = entityManager.View<TransformComponent, PhysicComponent2D>();
	for (auto entt : view)
	{
		Entity entity(entityManager, entt);
		if (entity.IsValid())
		{
			auto& physicComponent = entity.Get<PhysicComponent2D>();
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

void PhysicSystem2D::AfterUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
	auto& entityManager = mWorld->GetEntityManager();
	auto view = entityManager.View<TransformComponent, PhysicComponent2D>();
	for (auto entt : view)
	{
		Entity entity(entityManager, entt);
		if (entity.IsValid())
		{
			const auto& physicComponent = entity.Get<PhysicComponent2D>();
			if (physicComponent.GetBodyType() != PhysicBodyType::Static)
			{
				if (const auto* body = physicComponent.GetBody())
				{
					const auto& position = body->GetPosition();
					auto& transformComponent = entity.Get<TransformComponent>();
					// TODO : Fix this
					transformComponent.SetPosition(Vector3f(position.x, position.y, transformComponent.GetPosition().z));
					//transformComponent.SetRotation(Math::RadToDeg(body->GetAngle()));
				}
			}
		}
	}
}

void PhysicSystem2D::DisableGravity()
{
	SetGravity(Vector2f(0.0f, 0.0f));
}

void PhysicSystem2D::SetGravity(const Vector2f& gravity)
{
	mPhysicWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

Vector2f PhysicSystem2D::GetGravity() const
{
	const b2Vec2 gravity = mPhysicWorld->GetGravity();
	return Vector2f(gravity.x, gravity.y);
}

void PhysicSystem2D::SetVelocityIterations(U32 value)
{
	mVelocityIterations = value;
}

U32 PhysicSystem2D::GetVelocityIterations() const
{
	return mVelocityIterations;
}

void PhysicSystem2D::SetPositionIterations(U32 value)
{
	mPositionIterations = value;
}

U32 PhysicSystem2D::GetPositionIterations() const
{
	return mPositionIterations;
}

#if defined(ENLIVE_DEBUG)
void PhysicSystem2D::Render()
{
	ENLIVE_PROFILE_FUNCTION();
	if (mDebugRender)
	{
		mPhysicWorld->DrawDebugData();
	}
}

void PhysicSystem2D::SetDebugRendering(bool value)
{
	mDebugRender = value;
}

bool PhysicSystem2D::IsDebugRendering() const
{
	return mDebugRender;
}

void PhysicSystem2D::SetDebugRenderFlags(U32 flags)
{
	mInternalSystem.SetFlags(flags);
	mDebugRenderFlags = flags;
}

U32 PhysicSystem2D::GetDebugRenderFlags() const
{
	return mDebugRenderFlags;
}

bool PhysicSystem2D::Serialize(Serializer& serializer, const char* name)
{
	return GenericSerialization(serializer, name, *this);
}

bool PhysicSystem2D::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<PhysicSystem2D>::GetName(), TypeInfo<PhysicSystem2D>::GetHash()))
	{
		bool ret = false;
		auto gravity = GetGravity();
		if (GenericEdit(objectEditor, "Gravity", gravity))
		{
			SetGravity(gravity);
			ret = true;
		}
		ret = GenericEdit(objectEditor, "PositionIterations", mPositionIterations) || ret;
		ret = GenericEdit(objectEditor, "VelocityIterations", mVelocityIterations) || ret;

#ifdef ENLIVE_DEBUG
#ifdef ENLIVE_ENABLE_IMGUI
		if (objectEditor.IsImGuiEditor())
		{
			if (IsDebugRendering())
			{
				if (ImGui::Button("HideDebug"))
				{
					SetDebugRendering(false);
					ret = true;
				}
			}
			else
			{
				if (ImGui::Button("ShowDebug"))
				{
					SetDebugRendering(true);
					ret = true;
				}
			}
			// TODO : DebugRenderFlags
			/*
			if (IsDebugRendering())
			{
				auto renderFlags = GetDebugRenderFlags();
				if (false)
				{
					SetDebugRenderFlags(renderFlags);
					ret = true;
				}
			}
			*/
		}
#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_DEBUG

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

#endif // ENLIVE_DEBUG

b2Body* PhysicSystem2D::GetComponentBody(const PhysicComponent2D& component)
{
	return const_cast<b2Body*>(component.GetBody());
}

void PhysicSystem2D::InternalSystem::BeginContact(b2Contact* contact)
{
	if (contact != nullptr)
	{
		b2Fixture* fA = contact->GetFixtureA();
		b2Fixture* fB = contact->GetFixtureB();

		if (fA != nullptr)
		{
			if (b2Body* bA = fA->GetBody())
			{
				mSystem.mContactSignals[bA].BeginContact(contact, fA, fB);
			}
		}

		if (fB != nullptr)
		{
			if (b2Body* bB = fB->GetBody())
			{
				mSystem.mContactSignals[bB].BeginContact(contact, fB, fA);
			}
		}
	}
}

void PhysicSystem2D::InternalSystem::EndContact(b2Contact* contact)
{
	if (contact != nullptr)
	{
		b2Fixture* fA = contact->GetFixtureA();
		b2Fixture* fB = contact->GetFixtureB();

		if (fA != nullptr)
		{
			if (b2Body* bA = fA->GetBody())
			{
				mSystem.mContactSignals[bA].EndContact(contact, fA, fB);
			}
		}

		if (fB != nullptr)
		{
			if (b2Body* bB = fB->GetBody())
			{
				mSystem.mContactSignals[bB].EndContact(contact, fB, fA);
			}
		}
	}
}

#ifdef ENLIVE_DEBUG
void PhysicSystem2D::InternalSystem::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
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
		mSystem.mWorld->GetDebugDraw().DrawLine(pos1, pos2, c);
	}
}

void PhysicSystem2D::InternalSystem::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	// TODO : Fill draw
	DrawPolygon(vertices, vertexCount, color);
}

void PhysicSystem2D::InternalSystem::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
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

void PhysicSystem2D::InternalSystem::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	ENLIVE_UNUSED(axis);

	// TODO : Fill draw
	DrawCircle(center, radius, color);
}

void PhysicSystem2D::InternalSystem::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	const Vector3f pos1(p1.x, p1.y, 0.0f);
	const Vector3f pos2(p2.x, p2.y, 0.0f);
	Color c;
	c.FromBox2DColor(color);
	mSystem.mWorld->GetDebugDraw().DrawLine(pos1, pos2, c);
}

void PhysicSystem2D::InternalSystem::DrawTransform(const b2Transform& xf)
{
	constexpr F32 lineLength = 0.4f;
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetXAxis(), Colors::Red.ToBox2DColor());
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetYAxis(), Colors::Green.ToBox2DColor());
}

void PhysicSystem2D::InternalSystem::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	ENLIVE_UNUSED(size);

	const Vector3f pos(p.x, p.y, 0.0f);
	Color c;
	c.FromBox2DColor(color);
	mSystem.mWorld->GetDebugDraw().DrawPoint(pos, c);
}
#endif // ENLIVE_DEBUG

} // namespace en