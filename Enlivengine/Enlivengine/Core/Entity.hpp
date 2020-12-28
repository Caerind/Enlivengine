#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/TypeTraits.hpp>

#include <Enlivengine/Math/Vector3.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>

namespace en
{

class EntityManager;
class World;

// This is only a handle of an entity in a registry
// It does not create or destroy entity from registry
// It is used for CustomEditor/Serialization and convenience
class Entity
{
public:
	Entity();
	Entity(EntityManager& manager, entt::entity entity);
	Entity(World& world, entt::entity entity);

	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;

	bool IsValid() const;
	U32 GetID() const;
	void Destroy();

	const char* GetName() const;
	Vector3f GetPosition() const;

	template <typename T> 
	decltype(auto) Add();

	template <typename T, typename ...Args> 
	decltype(auto) Add(Args&&  ...args);

	template <typename ...Components> 
	bool Has() const;

	template <typename T> 
	void Remove();

	template <typename T> 
	T& Get();

	template <typename T> 
	const T& Get() const;

	bool HasManager() const;
	EntityManager& GetManager();
	const EntityManager& GetManager() const;

	World& GetWorld();
	const World& GetWorld() const;

	// TODO : Try to remove/hide these
	const entt::entity& GetEntity() const;
	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

private:
	friend class EntityManager;

private:
	EntityManager* mManager;
	entt::entity mEntity;
};

template <typename T> 
decltype(auto) Entity::Add()
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());

	if constexpr (Traits::IsEmpty<T>::value)
	{
		GetRegistry().emplace<T>(mEntity);
	}
	else
	{
		T& component = GetRegistry().emplace<T>(mEntity);
		if constexpr (ComponentHasCustomInitialization<T>::value)
		{
			ComponentHasCustomInitialization<T>::Initialize(*this, component);
		}
		return component;
	}
}

template <typename T, typename ...Args>
decltype(auto) Entity::Add(Args&& ...args)
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());

	if constexpr (Traits::IsEmpty<T>::value)
	{
		GetRegistry().emplace<T>(mEntity);
	}
	else
	{
		T& component = GetRegistry().emplace<T>(mEntity, std::forward<Args>(args)...);
		if constexpr (ComponentHasCustomInitialization<T>::value)
		{
			ComponentHasCustomInitialization<T>::Initialize(*this, component);
		}
		return component;
	}
}

template <typename ...Components> 
bool Entity::Has() const 
{ 
	enAssert(IsValid());

	return GetRegistry().has<Components...>(mEntity); 
}

template <typename T> 
void Entity::Remove()
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	enAssert(Has<T>());
	
	GetRegistry().remove<T>(mEntity); 
}

template <typename T> 
T& Entity::Get()
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	enAssert(Has<T>());

	return GetRegistry().get<T>(mEntity); 
}

template <typename T> 
const T& Entity::Get() const 
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	enAssert(Has<T>());

	return GetRegistry().get<T>(mEntity); 
}

} // namespace en