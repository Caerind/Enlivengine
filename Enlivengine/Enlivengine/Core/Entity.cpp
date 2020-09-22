#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

Entity::Entity()
	: mManager(nullptr)
	, mEntity(entt::null)
{
}

Entity::Entity(EntityManager& manager, entt::entity entity)
	: mManager(&manager)
	, mEntity(entity)
{
}

Entity::Entity(World& world, entt::entity entity)
	: mManager(&world.GetEntityManager())
	, mEntity(entity)
{

}

bool Entity::IsValid() const
{
	return mManager != nullptr && GetRegistry().valid(mEntity);
}

U32 Entity::GetID() const
{
	return static_cast<U32>(entt::to_integral(mEntity));
}

void Entity::Destroy()
{
	enAssert(IsValid());
	mManager->DestroyEntity(*this);
}

const char* Entity::GetName() const
{
	if (!IsValid() || !Has<NameComponent>())
	{
		return "";
	}
	else
	{
		return Get<NameComponent>().name.c_str();
	}
}

Vector2f Entity::GetPosition2D() const
{
	if (!IsValid() || !Has<TransformComponent>())
	{
		return Vector2f();
	}
	else
	{
		return Get<TransformComponent>().transform.GetPosition2D();
	}
}

Vector3f Entity::GetPosition() const
{
	if (!IsValid() || !Has<TransformComponent>())
	{
		return Vector3f();
	}
	else
	{
		return Get<TransformComponent>().transform.GetPosition();
	}
}

bool Entity::HasManager() const
{
	return mManager != nullptr;
}

EntityManager& Entity::GetManager()
{
	enAssert(mManager != nullptr);
	return *mManager;
}

const EntityManager& Entity::GetManager() const
{
	enAssert(mManager != nullptr);
	return *mManager;
}

World& Entity::GetWorld()
{
	enAssert(mManager != nullptr);
	return mManager->GetWorld();
}

const World& Entity::GetWorld() const
{
	enAssert(mManager != nullptr);
	return mManager->GetWorld();
}

const entt::entity& Entity::GetEntity() const
{
	return mEntity;
}

entt::registry& Entity::GetRegistry()
{
	enAssert(mManager != nullptr);
	return mManager->GetRegistry();
}

const entt::registry& Entity::GetRegistry() const
{
	enAssert(mManager != nullptr);
	return mManager->GetRegistry();
}

} // namespace en