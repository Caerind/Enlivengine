#include "PhysicComponent.hpp"

#include "World.hpp"
#include "Entity.hpp"

namespace en
{

b2BodyType ToB2BodyType(PhysicBodyType type)
{
	switch (type)
	{
	case PhysicBodyType::Dynamic: return b2_dynamicBody; break;
	case PhysicBodyType::Static: return b2_staticBody; break;
	case PhysicBodyType::Kinematic: return b2_kinematicBody; break;
	default: enAssert(false);
	}
	return b2_dynamicBody;
}

PhysicBodyType FromB2BodyType(b2BodyType type)
{
	switch (type)
	{
	case b2_dynamicBody: return PhysicBodyType::Dynamic; break;
	case b2_staticBody: return PhysicBodyType::Static; break;
	case b2_kinematicBody: return PhysicBodyType::Kinematic; break;
	default: enAssert(false);
	}
	return PhysicBodyType::Dynamic;
}

b2Shape::Type ToB2ShapeType(PhysicShapeType type)
{
	switch (type)
	{
	case PhysicShapeType::Circle: return b2Shape::Type::e_circle; break;
	case PhysicShapeType::Edge: return b2Shape::Type::e_edge; break;
	case PhysicShapeType::Polygon: return b2Shape::Type::e_polygon; break;
	case PhysicShapeType::Chain: return b2Shape::Type::e_chain; break;
	default: enAssert(false);
	}
	return b2Shape::Type::e_circle;
}

PhysicShapeType FromB2ShapeType(b2Shape::Type type)
{
	switch (type)
	{
	case b2Shape::Type::e_circle: return PhysicShapeType::Circle; break;
	case b2Shape::Type::e_edge: return PhysicShapeType::Edge; break;
	case b2Shape::Type::e_polygon: return PhysicShapeType::Polygon; break;
	case b2Shape::Type::e_chain: return PhysicShapeType::Chain; break;
	default: enAssert(false);
	}
	return PhysicShapeType::Circle;
}

PhysicComponent::PhysicComponent()
	: mEntity()
	, mBody(nullptr)
{
}

PhysicComponent::PhysicComponent(PhysicComponent&& other) noexcept
	: mEntity(other.mEntity)
	, mBody(other.mBody)
{
	other.mEntity = Entity();
	other.mBody = nullptr;
}

PhysicComponent::~PhysicComponent()
{
	if (mEntity.IsValid())
	{
		en::World& world = const_cast<en::World&>(mEntity.GetWorld());
		if (world.HasPhysicSystem())
		{
			world.GetPhysicSystem()->Deinitialize(mEntity, *this);
		}
	}
}

PhysicComponent& PhysicComponent::operator=(PhysicComponent&& other) noexcept
{
	mEntity = other.mEntity;
	mBody = other.mBody;
	other.mEntity = Entity();
	other.mBody = nullptr;
	return *this;
}

bool PhysicComponent::IsValid() const
{
	return mBody != nullptr;
}

void PhysicComponent::SetBodyType(PhysicBodyType type)
{
	if (IsValid())
	{
		mBody->SetType(ToB2BodyType(type));
	}
}

PhysicBodyType PhysicComponent::GetBodyType() const
{
	if (IsValid())
	{
		return FromB2BodyType(mBody->GetType());
	}
	else
	{
		return PhysicBodyType::Static;
	}
}

void PhysicComponent::SetLinearVelocity(const Vector2f& velocity)
{
	if (IsValid())
	{
		mBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}
}

Vector2f PhysicComponent::GetLinearVelocity() const
{
	if (IsValid())
	{
		const b2Vec2 velocity = mBody->GetLinearVelocity();
		return Vector2f(velocity.x, velocity.y);
	}
	else
	{
		return Vector2f(0.0f, 0.0f);
	}
}

void PhysicComponent::SetAngularVelocity(F32 velocity)
{
	if (IsValid())
	{
		mBody->SetAngularVelocity(velocity);
	}
}

F32 PhysicComponent::GetAngularVelocity() const
{
	if (IsValid())
	{
		return mBody->GetAngularVelocity();
	}
	else
	{
		return 0.0f;
	}
}

void PhysicComponent::SetGravityScale(F32 scale)
{
	if (IsValid())
	{
		mBody->SetGravityScale(scale);
	}
}

F32 PhysicComponent::GetGravityScale() const
{
	if (IsValid())
	{
		return mBody->GetGravityScale();
	}
	else
	{
		return 1.0f;
	}
}

void PhysicComponent::SetLinearDamping(F32 value)
{
	if (IsValid())
	{
		mBody->SetLinearDamping(value);
	}
}

F32 PhysicComponent::GetLinearDamping() const
{
	if (IsValid())
	{
		return mBody->GetLinearDamping();
	}
	else
	{
		return 0.0f;
	}
}

void PhysicComponent::SetAngularDamping(F32 value)
{
	if (IsValid())
	{
		mBody->SetAngularDamping(value);
	}
}

F32 PhysicComponent::GetAngularDamping() const
{
	if (IsValid())
	{
		return mBody->GetAngularDamping();
	}
	else
	{
		return 0.0f;
	}
}

void PhysicComponent::SetFixedRotation(bool value)
{
	if (IsValid())
	{
		mBody->SetFixedRotation(value);
	}
}

bool PhysicComponent::IsFixedRotation() const
{
	if (IsValid())
	{
		return mBody->IsFixedRotation();
	}
	else
	{
		return false;
	}
}

void PhysicComponent::SetBullet(bool value)
{
	if (IsValid())
	{
		mBody->SetBullet(value);
	}
}

bool PhysicComponent::IsBullet() const
{
	if (IsValid())
	{
		return mBody->IsBullet();
	}
	else
	{
		return false;
	}
}

en::F32 PhysicComponent::GetMass() const
{
	if (IsValid())
	{
		return mBody->GetMass();
	}
	else
	{
		return 0.0f;
	}
}

b2Body* PhysicComponent::GetBody()
{
	return mBody;
}

const b2Body* PhysicComponent::GetBody() const
{
	return mBody;
}

Entity PhysicComponent::GetEntity() const
{
	return mEntity;
}

World* PhysicComponent::GetWorld()
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

const World* PhysicComponent::GetWorld() const
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

} // namespace en