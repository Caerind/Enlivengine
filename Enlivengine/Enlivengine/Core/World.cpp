#include <Enlivengine/Core/World.hpp>

#include <Enlivengine/Utils/Profiler.hpp>

#include <Enlivengine/Resources/PathManager.hpp>

#include <Enlivengine/Core/Entity.hpp>

namespace en
{

World::World(const std::string& name)
	: mEntityManager(*this)
	, mSystems()
	, mPhysicSystem(nullptr)
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
	for (System* system : mSystems)
	{
		enDelete(System, system);
	}
}

EntityManager& World::GetEntityManager()
{
	return mEntityManager;
}

const EntityManager& World::GetEntityManager() const
{
	return mEntityManager;
}

PhysicSystemBase* World::GetPhysicSystem()
{
	return mPhysicSystem;
}

const PhysicSystemBase* World::GetPhysicSystem() const
{
	return mPhysicSystem;
}

bool World::HasPhysicSystem() const
{
	return mPhysicSystem != nullptr;
}

void World::Update(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
#ifdef ENLIVE_DEBUG
	if (mPlaying)
#endif // ENLIVE_DEBUG
	{
		for (System* system : mSystems)
		{
			ENLIVE_PROFILE_SCOPE(system->GetName());
			system->Update(dt);
		}
	}
}

void World::Render()
{
	ENLIVE_PROFILE_FUNCTION();
	for (System* system : mSystems)
	{
		ENLIVE_PROFILE_SCOPE(system->GetName());
		system->Render();
	}
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

bool World::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<World>::GetName(), TypeInfo<World>::GetHash()))
	{
		bool ret = true;

		ret = GenericSerialization(serializer, "EntityManager", mEntityManager) && ret;
		ret = GenericSerialization(serializer, "Systems", mSystems) && ret;

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
		ret = GenericEdit(objectEditor, "Systems", mSystems) || ret;

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

#endif // ENLIVE_DEBUG

} // namespace en