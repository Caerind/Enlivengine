#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Core/ComponentFactory.hpp>

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

bool Entity::operator==(const Entity& other) const
{
	return mManager == other.mManager && mEntity == other.mEntity;
}

bool Entity::operator!=(const Entity& other) const
{
	return !operator==(other);
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

Vector3f Entity::GetPosition() const
{
	if (!IsValid() || !Has<TransformComponent>())
	{
		return Vector3f();
	}
	else
	{
		return Get<TransformComponent>().GetPosition();
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

bool Entity::Serialize(ClassSerializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Entity>::GetHash()))
	{
		bool ret = true;

		if (serializer.IsReading())
		{
			const auto& componentInfos = ComponentFactory::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (serializer.HasNode(ci.name))
				{
					ci.add(*this);
					ret = ci.serialize(serializer, *this) && ret;
				}
			}
		}
		else if (serializer.IsWriting())
		{
			const auto& componentInfos = ComponentFactory::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (ci.has(*this))
				{
					ret = ci.serialize(serializer, *this) && ret;
				}
			}
		}
		else
		{ 
			enAssert(false);
			ret = false;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
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