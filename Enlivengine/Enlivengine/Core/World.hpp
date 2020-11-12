#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/Array.hpp>
#include <Enlivengine/Graphics/Camera.hpp>
#include <Enlivengine/Graphics/DebugDraw.hpp>
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

	Camera& GetMainCamera();
	const Camera& GetMainCamera() const;

#ifdef ENLIVE_DEBUG
	Camera& GetFreeCamera();
	const Camera& GetFreeCamera() const;

	DebugDraw& GetDebugDraw();
#endif // ENLIVE_DEBUG

	void Play();
	void Pause();
	bool IsPlaying() const;
	
	void Update(Time dt);
	void Render();

private:
	EntityManager mEntityManager;

	std::vector<System*> mSystems;
	PhysicSystem* mPhysicSystem;

	Camera mMainCamera;

#ifdef ENLIVE_DEBUG
	Camera mFreeCamera;
	DebugDraw mDebugDraw;
#endif // ENLIVE_DEBUG

	bool mPlaying;
};

template <typename T, typename ... Args>
T* World::CreateSystem(Args&& ... args)
{
	static_assert(Traits::IsBaseOf<System, T>::value);

	T* system = new T(*this, std::forward<Args>(args)...);
	mSystems.push_back(system);

	if constexpr (Traits::IsBaseOf<PhysicSystem, T>::value)
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

#endif // ENLIVE_MODULE_CORE