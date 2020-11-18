#pragma once

#include <Box2D/Box2D.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

enum class PhysicBodyType
{
	Dynamic,
	Static,
	Kinematic
};
b2BodyType ToB2BodyType(PhysicBodyType type);
PhysicBodyType FromB2BodyType(b2BodyType type);

enum class PhysicShapeType
{
	Circle,
	Edge,
	Polygon,
	Chain
};
b2Shape::Type ToB2ShapeType(PhysicShapeType type);
PhysicShapeType FromB2ShapeType(b2Shape::Type type);

class PhysicComponent
{
    public:
		PhysicComponent();
		PhysicComponent(PhysicComponent&& other) noexcept;
		PhysicComponent(const PhysicComponent&) = delete;
		~PhysicComponent();

		PhysicComponent& operator=(PhysicComponent&& other) noexcept;
		PhysicComponent& operator=(const PhysicComponent&) = delete;

		bool IsValid() const;
        
        void SetBodyType(PhysicBodyType type);
		PhysicBodyType GetBodyType() const;
        
        void SetLinearVelocity(const Vector2f& velocity);
        Vector2f GetLinearVelocity() const;
        
        void SetAngularVelocity(F32 velocity);
        F32 GetAngularVelocity() const;
        
        void SetGravityScale(F32 scale);
        F32 GetGravityScale() const;
        
        void SetLinearDamping(F32 value);
        F32 GetLinearDamping() const;
        
        void SetAngularDamping(F32 value);
        F32 GetAngularDamping() const;
        
        void SetFixedRotation(bool value);
        bool IsFixedRotation() const;
        
        void SetBullet(bool value);
        bool IsBullet() const;

		F32 GetMass() const;
        
        // TODO : AllowSleeping/Awake/Enable
        
        // TODO : Fixtures

		b2Body* GetBody();
		const b2Body* GetBody() const;

		Entity GetEntity() const;
		World* GetWorld();
		const World* GetWorld() const;

    private:
		friend class PhysicSystem;

		Entity mEntity;
        b2Body* mBody;
};

} // namespace en

template <>
struct ComponentHasCustomInitialization<en::PhysicComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, en::PhysicComponent& component)
	{
		enAssert(entity.IsValid());
		en::World& world = const_cast<en::World&>(entity.GetWorld());
		if (world.HasPhysicSystem())
		{
			return world.GetPhysicSystem()->Initialize(entity, component);
		}
		return true;
	}
};