#include "PhysicSystem.hpp"

#include <SFML/Graphics.hpp>

#include <Enlivengine/System/Profiler.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/PhysicComponent.hpp>

namespace en
{

PhysicSystem::PhysicSystem(World& world)
	: System(world)
	, mPhysicWorld(nullptr)
	, mPixelsPerMeter(32.0f)
	, mVelocityIterations(8)
	, mPositionIterations(3)
	, mPlaying(true)
#ifdef ENLIVE_DEBUG
	, mDebugRenderTarget(nullptr)
	, mDebugRenderFlags(static_cast<U32>(b2Draw::e_shapeBit))
	, mDebugRender(false)
#endif // ENLIVE_DEBUG
{
	mPhysicWorld = new b2World(b2Vec2(0.0f, 0.0f));
	enAssert(mPhysicWorld != nullptr);

#ifdef ENLIVE_DEBUG
	mPhysicWorld->SetDebugDraw(this);
	SetFlags(mDebugRenderFlags);
#endif // ENLIVE_DEBUG

	mPhysicWorld->SetContactListener(this);
}

PhysicSystem::~PhysicSystem()
{
	delete mPhysicWorld;
}

bool PhysicSystem::Initialize(const Entity& entity, PhysicComponent& component)
{
	if (!entity.IsValid())
	{
		return false;
	}

	static b2BodyDef bodyDef;
	if (entity.Has<TransformComponent>())
	{
		const Transform& transform = entity.Get<TransformComponent>().transform;
		bodyDef.position.Set(transform.GetPosition2D().x / mPixelsPerMeter, transform.GetPosition2D().y / mPixelsPerMeter);
		bodyDef.angle = Math::DegToRad(transform.GetRotation2D());
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
	if (mPlaying)
	{
		mPhysicWorld->Step(dt.AsSeconds(), mVelocityIterations, mPositionIterations);
	}
}

void PhysicSystem::Play()
{
	mPlaying = true;
}

void PhysicSystem::Pause()
{
	mPlaying = false;
}

bool PhysicSystem::IsPlaying() const
{
	return mPlaying;
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

void PhysicSystem::SetPixelsPerMeter(F32 value)
{
	mPixelsPerMeter = value;
}

F32 PhysicSystem::GetPixelsPerMeter() const
{
	return mPixelsPerMeter;
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

#ifdef ENLIVE_DEBUG
void PhysicSystem::Render(sf::RenderTarget& target)
{
	ENLIVE_PROFILE_FUNCTION();
	if (mDebugRender)
	{
		mDebugRenderTarget = &target;
		mPhysicWorld->DrawDebugData();
		mDebugRenderTarget = nullptr;
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
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		polygon.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y) * mPixelsPerMeter);
	}
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255)));
	polygon.setOutlineThickness(-1.0f);

	if (mDebugRenderTarget != nullptr)
	{
		mDebugRenderTarget->draw(polygon);
	}
}

void PhysicSystem::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		polygon.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y) * mPixelsPerMeter);
	}
	sf::Color c = sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255));
	c.a = 60;
	polygon.setFillColor(c);
	c.a = 255;
	polygon.setOutlineColor(c);
	polygon.setOutlineThickness(-1.0f);

	if (mDebugRenderTarget != nullptr)
	{
		mDebugRenderTarget->draw(polygon);
	}
}

void PhysicSystem::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
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
}

void PhysicSystem::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	ENLIVE_UNUSED(axis);

	sf::CircleShape circle(radius * mPixelsPerMeter);
	circle.setOrigin(radius * mPixelsPerMeter, radius * mPixelsPerMeter);
	circle.setPosition(sf::Vector2f(center.x, center.y) * mPixelsPerMeter);
	sf::Color c = sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255));
	c.a = 60;
	circle.setFillColor(c);
	c.a = 255;
	circle.setOutlineColor(c);
	circle.setOutlineThickness(-1.0f);

	if (mDebugRenderTarget != nullptr)
	{
		mDebugRenderTarget->draw(circle);
	}
}

void PhysicSystem::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	const sf::Color c = sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255));
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y) * mPixelsPerMeter, c),
		sf::Vertex(sf::Vector2f(p2.x, p2.y) * mPixelsPerMeter, c)
	};

	if (mDebugRenderTarget != nullptr)
	{
		mDebugRenderTarget->draw(line, 2, sf::Lines);
	}
}

void PhysicSystem::DrawTransform(const b2Transform& xf)
{
	constexpr F32 lineLength = 0.4f;
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetXAxis(), b2Color(1.f, 0.f, 0.f));
	DrawSegment(xf.p, xf.p + lineLength * xf.q.GetYAxis(), b2Color(0.f, 1.f, 0.f));
}

void PhysicSystem::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	ENLIVE_UNUSED(size);

	static constexpr en::F32 radius = 5.0f;

	sf::CircleShape circle(radius);
	circle.setOrigin(radius, radius);
	circle.setPosition(p.x * mPixelsPerMeter, p.y * mPixelsPerMeter);
	circle.setFillColor(sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255)));

	if (mDebugRenderTarget != nullptr)
	{
		mDebugRenderTarget->draw(circle);
	}
}
#endif // ENLIVE_DEBUG

} // namespace en
