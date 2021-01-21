#pragma once

#include <Box2D/Box2D.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
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

class PhysicComponent2D
{
public:
	PhysicComponent2D();
	PhysicComponent2D(PhysicComponent2D&& other) noexcept;
	PhysicComponent2D(const PhysicComponent2D&) = delete;
	~PhysicComponent2D();

	PhysicComponent2D& operator=(PhysicComponent2D&& other) noexcept;
	PhysicComponent2D& operator=(const PhysicComponent2D&) = delete;

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

	bool Initialize(const Entity& entity);

private:
	friend class PhysicSystem2D;

	Entity mEntity;
    b2Body* mBody;
};

} // namespace en

ENLIVE_DEFINE_ENUM_INFO(en::PhysicBodyType)

ENLIVE_DEFINE_ENUM_INFO(en::PhysicShapeType)

ENLIVE_META_CLASS_BEGIN(en::PhysicComponent2D, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("type", &en::PhysicComponent2D::GetBodyType, &en::PhysicComponent2D::SetBodyType),
	ENLIVE_META_CLASS_MEMBER("linearVelocity", &en::PhysicComponent2D::GetLinearVelocity, &en::PhysicComponent2D::SetLinearVelocity),
	ENLIVE_META_CLASS_MEMBER("angularVelocity", &en::PhysicComponent2D::GetAngularVelocity, &en::PhysicComponent2D::SetAngularVelocity),
	ENLIVE_META_CLASS_MEMBER("gravityScale", &en::PhysicComponent2D::GetGravityScale, &en::PhysicComponent2D::SetGravityScale),
	ENLIVE_META_CLASS_MEMBER("linearDamping", &en::PhysicComponent2D::GetLinearDamping, &en::PhysicComponent2D::SetLinearDamping),
	ENLIVE_META_CLASS_MEMBER("angularDamping", &en::PhysicComponent2D::GetAngularDamping, &en::PhysicComponent2D::SetAngularDamping),
	ENLIVE_META_CLASS_MEMBER("fixedRotation", &en::PhysicComponent2D::IsFixedRotation, &en::PhysicComponent2D::SetFixedRotation),
	ENLIVE_META_CLASS_MEMBER("bullet", &en::PhysicComponent2D::IsBullet, &en::PhysicComponent2D::SetBullet)
ENLIVE_META_CLASS_END()
ENLIVE_COMPONENT_CUSTOM_INITIALIZATION(en::PhysicComponent2D)