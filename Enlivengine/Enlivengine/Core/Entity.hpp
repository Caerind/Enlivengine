#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/System/Meta.hpp>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>

#include <Enlivengine/Core/CustomTraits.hpp>

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

	bool IsValid() const;
	U32 GetID() const;
	void Destroy();

	const char* GetName() const;
	Vector2f GetPosition2D() const;
	Vector3f GetPosition() const;

	template <typename T> 
	T& Add();

	template <typename T, typename ...Args> 
	T& Add(Args&&  ...args);

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
T& Entity::Add()
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());

	T& component = GetRegistry().emplace<T>(mEntity);
	if constexpr (CustomComponentInitialization<T>::value)
	{
		CustomComponentInitialization<T>::Initialize(*this, component);
	}
	return component;
}

template <typename T, typename ...Args>
T& Entity::Add(Args&& ...args)
{
	//enAssert(ComponentManager::IsRegistered<T>());
	enAssert(IsValid());
	
	T& component = GetRegistry().emplace<T>(mEntity, std::forward<Args>(args)...); 
	if constexpr (CustomComponentInitialization<T>::value)
	{
		CustomComponentInitialization<T>::Initialize(*this, component);
	}
	return component;
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

ENLIVE_META_CLASS_BEGIN(en::Entity)
ENLIVE_META_CLASS_END()
