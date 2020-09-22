#pragma once

#include <Enlivengine/System/Array.hpp>
#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/System.hpp>
#include <Enlivengine/Core/PhysicSystem.hpp>

namespace en
{

class World
{
public:
	World();
	~World();

	EntityManager& GetEntityManager();
	const EntityManager& GetEntityManager() const;

	template <typename T, typename ... Args>
	T* CreateSystem(Args&& ... args);

	bool HasPhysicSystem() const;
	PhysicSystem* GetPhysicSystem();
	const PhysicSystem* GetPhysicSystem() const;

	View& GetGameView();
	const View& GetGameView() const;

#ifdef ENLIVE_DEBUG
	View& GetFreeCamView();
	const View& GetFreeCamView() const;
#endif // ENLIVE_DEBUG

	void Play();
	void Pause();
	bool IsPlaying() const;
	
	void Update(Time dt);
	void Render(sf::RenderTarget& target);

private:
	EntityManager mEntityManager;

	std::vector<System*> mSystems;
	PhysicSystem* mPhysicSystem;

	View mGameView;

#ifdef ENLIVE_DEBUG
	View mFreeCamView;
#endif // ENLIVE_DEBUG

	bool mPlaying;
};

template <typename T, typename ... Args>
T* en::World::CreateSystem(Args&& ... args)
{
	static_assert(en::Traits::IsBaseOf<en::System, T>::value);

	T* system = new T(*this, std::forward<Args>(args)...);
	mSystems.push_back(system);

	if constexpr (en::Traits::IsBaseOf<en::PhysicSystem, T>::value)
	{
		if (mPhysicSystem != nullptr)
		{
			enLogWarning(LogChannel::Core, "World have too many PhysicSystems");
		}
		mPhysicSystem = system;
	}

	return system;
}

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::World)
	ENLIVE_META_CLASS_MEMBER("EntityManager", &en::World::GetEntityManager)
ENLIVE_META_CLASS_END()
