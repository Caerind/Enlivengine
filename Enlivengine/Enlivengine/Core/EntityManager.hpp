#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/CustomTraits.hpp>

namespace en
{

class World;

class EntityManager
{
public:
	EntityManager(World& world);

	Entity CreateEntity();
	void DestroyEntity(Entity& entity);
	void ClearEntities();

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

	// TODO : Try to remove/hide these
	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

private:
	friend class Entity;

private:
	World& mWorld;
	entt::registry mRegistry;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::EntityManager)
ENLIVE_META_CLASS_END()