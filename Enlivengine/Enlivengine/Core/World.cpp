#include <Enlivengine/Core/World.hpp>

#ifdef ENLIVE_MODULE_CORE

namespace en
{

World::World()
	: mEntityManager(*this)
	, mSystems()
	, mPhysicSystem(nullptr)
	, mMainCamera(nullptr)
	, mPlaying(false)
#ifdef ENLIVE_DEBUG
	, mDebugDraw()
	, mSelectedEntities()
#endif // ENLIVE_DEBUG
{
}

World::~World()
{
	for (System* system : mSystems)
	{
		delete system;
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

PhysicSystem* World::GetPhysicSystem()
{
	return mPhysicSystem;
}

const PhysicSystem* World::GetPhysicSystem() const
{
	return mPhysicSystem;
}

bool World::HasPhysicSystem() const
{
	return mPhysicSystem != nullptr;
}

Camera* World::GetMainCamera() const
{
	return mMainCamera;
}

void World::SetMainCamera(Camera* camera)
{
	mMainCamera = camera;
}

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

void World::Update(Time dt)
{
	for (System* system : mSystems)
	{
		system->Update(dt);
	}
}

void World::Render()
{
	for (System* system : mSystems)
	{
		system->Render();
	}
}

#ifdef ENLIVE_DEBUG
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

const std::vector<entt::entity>& World::GetSelectedEntities() const
{
	return mSelectedEntities;
}
#endif // ENLIVE_DEBUG

} // namespace en

#endif // ENLIVE_MODULE_CORE