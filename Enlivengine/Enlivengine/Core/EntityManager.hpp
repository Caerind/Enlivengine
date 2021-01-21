#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/Utils/Meta.hpp>

namespace en
{

class World;
class Entity;
class ClassSerializer;

class EntityManager
{
public:
	EntityManager(World& world);

	Entity CreateEntity();
	void DestroyEntity(Entity& entity);
	void ClearEntities();

	U32 GetEntityCount() const;

	template <typename Func>
	void Each(Func func) const
	{
		mRegistry.each(func);
	}

	template <typename... Component, typename... Exclude>
	auto View(entt::exclude_t<Exclude...> exclusion = {}) const
	{
		return mRegistry.view<Component...>(exclusion);
	}

	template <typename... Component, typename... Exclude>
	auto View(entt::exclude_t<Exclude...> exclusion = {})
	{
		return mRegistry.view<Component...>(exclusion);
	}

	World& GetWorld();
	const World& GetWorld() const;

	bool Serialize(ClassSerializer& serializer, const char* name);

	// TODO : Try to remove/hide these
	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

private:
	World& mWorld;
	entt::registry mRegistry;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::EntityManager, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()