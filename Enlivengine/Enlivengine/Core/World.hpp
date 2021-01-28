#pragma once

#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/SystemManager.hpp>

#ifdef ENLIVE_DEBUG
#include <Enlivengine/Graphics/DebugDraw.hpp>
#endif // ENLIVE_DEBUG

namespace en
{

class World
{
public:
	World(const std::string& name);
	~World();

	EntityManager& GetEntityManager();
	const EntityManager& GetEntityManager() const;

	SystemManager& GetSystemManager();
	const SystemManager& GetSystemManager() const;
	
	void UpdatePhysic();
	void Update();
	void UpdateTool();
	void Render();

	const std::string& GetName() const;

#ifdef ENLIVE_DEBUG
	void SetDebugRendering(bool value);
	bool IsDebugRendering() const;
	DebugDraw& GetDebugDraw();

	bool IsSelected(const Entity& entity) const;
	bool SelectEntity(const Entity& entity);
	bool UnselectEntity(const Entity& entity);
	void ClearSelectedEntities();
	const std::vector<entt::entity>& GetSelectedEntities() const;
#endif // ENLIVE_DEBUG

	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	EntityManager mEntityManager;
	SystemManager mSystemManager;

	std::string mName;

#ifdef ENLIVE_DEBUG
	bool mDebugRendering;
	DebugDraw mDebugDraw;

	std::vector<entt::entity> mSelectedEntities;
#endif // ENLIVE_DEBUG
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::World, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()