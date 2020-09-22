#include <Enlivengine/Core/World.hpp>

namespace en
{

World::World()
	: mEntityManager(*this)
	, mSystems()
	, mPhysicSystem(nullptr)
	, mGameView()
#ifdef ENLIVE_DEBUG
	, mFreeCamView()
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

View& World::GetGameView()
{
	return mGameView;
}

const View& World::GetGameView() const
{
	return mGameView;
}

#ifdef ENLIVE_DEBUG
View& World::GetFreeCamView()
{
	return mFreeCamView;
}

const View& World::GetFreeCamView() const
{
	return mFreeCamView;
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

void World::Render(sf::RenderTarget& target)
{
	for (System* system : mSystems)
	{
		system->Render(target);
	}
}

} // namespace en