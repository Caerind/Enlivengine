#pragma once

#include <Box2D/Box2D.h>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Core/System.hpp>

#include <Enlivengine/System/Signal.hpp>

namespace en
{

class World;
class Entity;
class PhysicComponent;

class PhysicSystem : public System, public b2ContactListener, public b2Draw
{
    public:
        PhysicSystem(World& world);
		~PhysicSystem();

		void Update(Time dt) override;

		bool Initialize(const Entity& entity, PhysicComponent& component);
		bool Deinitialize(const Entity& entity, PhysicComponent& component);
        
		void Play();
		void Pause();
		bool IsPlaying() const;
        
        void DisableGravity();
        void SetGravity(const Vector2f& gravity);
        Vector2f GetGravity() const;
        
        void SetPixelsPerMeter(F32 value);
        F32 GetPixelsPerMeter() const;
        
        void SetVelocityIterations(U32 value);
        U32 GetVelocityIterations() const;
        void SetPositionIterations(U32 value);
		U32 GetPositionIterations() const;

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

		using BeginContactSlotType = ::en::Signal<b2Contact*, b2Fixture*, b2Fixture*>::ConnectionGuard;
		using EndContactSlotType = ::en::Signal<b2Contact*, b2Fixture*, b2Fixture*>::ConnectionGuard;

		template <typename F>
		bool AddBeginContactSlot(BeginContactSlotType& slot, const PhysicComponent& component, F&& fct);
		template <typename F>
		bool AddEndContactSlot(EndContactSlotType& slot, const PhysicComponent& component, F&& fct);

#ifdef ENLIVE_DEBUG
		void Render(sf::RenderTarget& target) override;

		void SetDebugRendering(bool value);
		bool IsDebugRendering() const;

		void SetDebugRenderFlags(U32 flags);
		U32 GetDebugRenderFlags() const;

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf);
		void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
#endif // ENLIVE_DEBUG

    protected:
		b2World* mPhysicWorld;
        F32 mPixelsPerMeter;
        U32 mVelocityIterations;
		U32 mPositionIterations;
		bool mPlaying;

		struct ContactSignals
		{
			enSignal(BeginContact, b2Contact*, b2Fixture*, b2Fixture*);
			enSignal(EndContact, b2Contact*, b2Fixture*, b2Fixture*);
		};
		std::unordered_map<b2Body*, ContactSignals> mContactSignals;

#ifdef ENLIVE_DEBUG
		sf::RenderTarget* mDebugRenderTarget;
		U32 mDebugRenderFlags;
        bool mDebugRender;
#endif // ENLIVE_DEBUG
};

template <typename F>
bool PhysicSystem::AddBeginContactSlot(BeginContactSlotType& slot, const PhysicComponent& component, F&& fct)
{
	if (b2Body* body = component.mBody)
	{
		auto itr = mContactSignals.find(body);
		if (itr == mContactSignals.end())
		{
			itr = mContactSignals.emplace(std::make_pair(component.mBody, ContactSignals())).first;
		}
		if (itr != mContactSignals.end())
		{
			slot.Connect(itr->second.BeginContact, fct);
			return true;
		}
	}
	return false;
}

template <typename F>
bool PhysicSystem::AddEndContactSlot(EndContactSlotType& slot, const PhysicComponent& component, F&& fct)
{
	if (b2Body* body = component.mBody)
	{
		auto itr = mContactSignals.find(body);
		if (itr == mContactSignals.end())
		{
			itr = mContactSignals.emplace(std::make_pair(component.mBody, ContactSignals())).first;
		}
		if (itr != mContactSignals.end())
		{
			slot.Connect(itr->second.EndContact, fct);
			return true;
		}
	}
	return false;
}

} // namespace en