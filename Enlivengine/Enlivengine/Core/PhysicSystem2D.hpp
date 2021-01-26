#pragma once

#include <unordered_map>

#include <Box2D/Box2D.h>

#include <Enlivengine/Config.hpp>
#include <Enlivengine/Utils/Signal.hpp>
#include <Enlivengine/Math/Vector2.hpp>

#include <Enlivengine/Utils/ObjectEditor.hpp>

#include <Enlivengine/Core/PhysicSystemBase.hpp>

namespace en
{

class Entity;
class PhysicComponent2D;

class PhysicSystem2D : public PhysicSystemBase
{
public:
	PhysicSystem2D();
	~PhysicSystem2D();

	// TODO : Remove this
	const char* GetName() const override
	{
		return "en::PhysicSystem2D";
	}

	void Update(Time dt) override;
	virtual void BeforeUpdate();
	virtual void AfterUpdate();

	bool Initialize(const Entity& entity, PhysicComponent2D& component);
	bool Deinitialize(const Entity& entity, PhysicComponent2D& component);
        
    void DisableGravity();
    void SetGravity(const Vector2f& gravity);
    Vector2f GetGravity() const;
        
    void SetVelocityIterations(U32 value);
    U32 GetVelocityIterations() const;
    void SetPositionIterations(U32 value);
	U32 GetPositionIterations() const;

	using BeginContactSlotType = ::en::Signal<b2Contact*, b2Fixture*, b2Fixture*>::ConnectionGuard;
	using EndContactSlotType = ::en::Signal<b2Contact*, b2Fixture*, b2Fixture*>::ConnectionGuard;

	template <typename F>
	bool AddBeginContactSlot(BeginContactSlotType& slot, const PhysicComponent2D& component, F&& fct);
	template <typename F>
	bool AddEndContactSlot(EndContactSlotType& slot, const PhysicComponent2D& component, F&& fct);

#if defined(ENLIVE_DEBUG)
	void Render() override;

	void SetDebugRendering(bool value);
	bool IsDebugRendering() const;

	void SetDebugRenderFlags(U32 flags);
	U32 GetDebugRenderFlags() const;
#endif // ENLIVE_DEBUG

	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	static b2Body* GetComponentBody(const PhysicComponent2D& component);

	// Don't know why, but it seems Meta doesn't work well with multiple inheritance...
	// So I will do the multiple inheritance here and compose
	// (The bug doesn't seems to be on the MetaStuff from EliasDaler, so I should have made it in my version...)
#ifdef ENLIVE_DEBUG
	class InternalSystem : public b2ContactListener, public b2Draw
#else
	class InternalSystem : public b2ContactListener
#endif //ENLIVE_DEBUG
	{
	public:
		InternalSystem(PhysicSystem2D& system) : mSystem(system) {}

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

#if defined(ENLIVE_DEBUG)
		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
		void DrawTransform(const b2Transform& xf) override;
		void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;
#endif // ENLIVE_DEBUG

	private:
		PhysicSystem2D& mSystem;
	};
	friend class InternalSystem;

protected:
	b2World* mPhysicWorld;
	InternalSystem mInternalSystem;
    U32 mVelocityIterations;
	U32 mPositionIterations;

	struct ContactSignals
	{
		enSignal(BeginContact, b2Contact*, b2Fixture*, b2Fixture*);
		enSignal(EndContact, b2Contact*, b2Fixture*, b2Fixture*);
	};
	std::unordered_map<b2Body*, ContactSignals> mContactSignals;

#if defined(ENLIVE_DEBUG)
	U32 mDebugRenderFlags;
    bool mDebugRender;
#endif // ENLIVE_DEBUG
};

template <typename F>
bool PhysicSystem2D::AddBeginContactSlot(BeginContactSlotType& slot, const PhysicComponent2D& component, F&& fct)
{
	if (b2Body* body = GetComponentBody(component))
	{
		auto itr = mContactSignals.find(body);
		if (itr == mContactSignals.end())
		{
			itr = mContactSignals.emplace(std::make_pair(body, ContactSignals())).first;
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
bool PhysicSystem2D::AddEndContactSlot(EndContactSlotType& slot, const PhysicComponent2D& component, F&& fct)
{
	if (b2Body* body = GetComponentBody(component))
	{
		auto itr = mContactSignals.find(body);
		if (itr == mContactSignals.end())
		{
			itr = mContactSignals.emplace(std::make_pair(body, ContactSignals())).first;
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

ENLIVE_META_CLASS_BEGIN(en::PhysicSystem2D, en::Type_ClassSerialization, en::Type_CustomEditor)
	ENLIVE_META_CLASS_MEMBER("gravity", &en::PhysicSystem2D::GetGravity, &en::PhysicSystem2D::SetGravity),
	ENLIVE_META_CLASS_MEMBER("positionIterations", &en::PhysicSystem2D::GetPositionIterations, &en::PhysicSystem2D::SetPositionIterations),
	ENLIVE_META_CLASS_MEMBER("velocityIterations", &en::PhysicSystem2D::GetVelocityIterations, &en::PhysicSystem2D::SetVelocityIterations)
ENLIVE_META_CLASS_END()