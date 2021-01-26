#include <Enlivengine/Core/World.hpp>

#include <Enlivengine/Utils/Profiler.hpp>

#include <Enlivengine/Resources/PathManager.hpp>

#include <Enlivengine/Core/Entity.hpp>

namespace en
{

World::World(const std::string& name)
	: mEntityManager(*this)
	, mSystemManager(*this)
	, mName(name)
#ifdef ENLIVE_DEBUG
	, mPlaying(false)
	, mDebugDraw()
	, mSelectedEntities()
#endif // ENLIVE_DEBUG
{
}

World::~World()
{
}

EntityManager& World::GetEntityManager()
{
	return mEntityManager;
}

const EntityManager& World::GetEntityManager() const
{
	return mEntityManager;
}

SystemManager& World::GetSystemManager()
{
	return mSystemManager;
}

const SystemManager& World::GetSystemManager() const
{
	return mSystemManager;
}

void World::UpdatePhysic(Time dt)
{
#ifdef ENLIVE_DEBUG
	if (mPlaying)
#endif // ENLIVE_DEBUG
	{
		mSystemManager.UpdatePhysic(dt);
	}
}

void World::Update(Time dt)
{
#ifdef ENLIVE_DEBUG
	if (mPlaying)
#endif // ENLIVE_DEBUG
	{
		mSystemManager.Update(dt);
	}
}

void World::Render()
{
	mSystemManager.Render();
}

const std::string& World::GetName() const
{
	return mName;
}

#ifdef ENLIVE_DEBUG
void World::Play()
{
	mPlaying = true;
}

void World::Pause()
{
	mPlaying = false;
}

bool World::IsPlaying() const
{
	return mPlaying;
}

DebugDraw& World::GetDebugDraw()
{
	return mDebugDraw;
}

bool World::IsSelected(const Entity& entity) const
{
	if (entity.IsValid() && &entity.GetWorld() == this)
	{
		const auto& entityToTest = entity.GetEntity();
		for (const auto& ent : mSelectedEntities)
		{
			if (ent == entityToTest)
			{
				return true;
			}
		}
	}
	return false;
}

bool World::SelectEntity(const Entity& entity)
{
	if (entity.IsValid() && &entity.GetWorld() == this && !IsSelected(entity))
	{
		mSelectedEntities.push_back(entity.GetEntity());
		return true;
	}
	else
	{
		return false;
	}
}

bool World::UnselectEntity(const Entity& entity)
{
	if (entity.IsValid() && &entity.GetWorld() == this)
	{
		entt::entity entt = entity.GetEntity();
		U32 count = static_cast<U32>(mSelectedEntities.size());
		for (U32 i = 0; i < count; ++i)
		{
			if (entt == mSelectedEntities[i])
			{
				mSelectedEntities.erase(mSelectedEntities.begin() + i);
				return true;
			}
		}
	}
	return false;
}

void World::ClearSelectedEntities()
{
	mSelectedEntities.clear();
}

const std::vector<entt::entity>& World::GetSelectedEntities() const
{
	return mSelectedEntities;
}

#endif // ENLIVE_DEBUG

bool World::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<World>::GetName(), TypeInfo<World>::GetHash()))
	{
		bool ret = true;

		ret = GenericSerialization(serializer, "EntityManager", mEntityManager) && ret;
		ret = GenericSerialization(serializer, "SystemManager", mSystemManager) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool World::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<World>::GetName(), TypeInfo<World>::GetHash()))
	{
		bool ret = false;

		ret = GenericEdit(objectEditor, "EntityManager", mEntityManager) || ret;
		ret = GenericEdit(objectEditor, "SystemManager", mSystemManager) || ret;

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

} // namespace en