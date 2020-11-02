#include <Enlivengine/Core/World.hpp>

#ifdef ENLIVE_MODULE_CORE

namespace en
{

World::World()
	: mEntityManager(*this)
	, mSystems()
	, mPhysicSystem(nullptr)
	, mMainCamera()
#ifdef ENLIVE_DEBUG
	, mFreeCamera()
#endif // ENLIVE_DEBUG
	, mPlaying(false)
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

bool World::HasPhysicSystem() const
{
	return mPhysicSystem != nullptr;
}

PhysicSystem* World::GetPhysicSystem()
{
	return mPhysicSystem;
}

const PhysicSystem* World::GetPhysicSystem() const
{
	return mPhysicSystem;
}

Camera& World::GetMainCamera()
{
	return mMainCamera;
}

const Camera& World::GetMainCamera() const
{
	return mMainCamera;
}

#ifdef ENLIVE_DEBUG
Camera& World::GetFreeCamera()
{
	return mFreeCamera;
}

const Camera& World::GetFreeCamera() const
{
	return mFreeCamera;
}
#endif // ENLIVE_DEBUG

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

} // namespace en

#endif // ENLIVE_MODULE_CORE