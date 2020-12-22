#pragma once

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

	template <typename T>
	T* GetSystem();
	template <typename T>
	const T* GetSystem() const;
	template <typename T>
	bool HasSystem() const;

	PhysicSystem* GetPhysicSystem();
	const PhysicSystem* GetPhysicSystem() const;
	bool HasPhysicSystem() const;

	void Play();
	void Pause();
	bool IsPlaying() const;
	
	void Update(Time dt);
	void Render();

#ifdef ENLIVE_DEBUG
	DebugDraw& GetDebugDraw();

	bool IsSelected(const Entity& entity) const;
	bool SelectEntity(const Entity& entity);
	bool UnselectEntity(const Entity& entity);
	void ClearSelectedEntities();
	const std::vector<entt::entity>& GetSelectedEntities() const;
#endif // ENLIVE_DEBUG

private:
	EntityManager mEntityManager;

	std::vector<System*> mSystems;
	PhysicSystem* mPhysicSystem;

	bool mPlaying;

#ifdef ENLIVE_DEBUG
	DebugDraw mDebugDraw;

	std::vector<entt::entity> mSelectedEntities;
#endif // ENLIVE_DEBUG
};

template <typename T, typename ... Args>
T* World::CreateSystem(Args&& ... args)
{
	static_assert(Traits::IsBaseOf<System, T>::value);

	if (T* system = GetSystem<T>())
	{
		enLogWarning(LogChannel::Core, "World have too many {}", TypeInfo<T>::GetName());
		return system;
	}
	else
	{
		system = new T(*this, std::forward<Args>(args)...);
		if (system != nullptr)
		{
			mSystems.push_back(system);

			if constexpr (Traits::IsBaseOf<PhysicSystem, T>::value)
			{
				mPhysicSystem = system;
			}
		}
		return system;
	}
}

template <typename T>
T* World::GetSystem()
{
	static_assert(Traits::IsBaseOf<System, T>::value);
	for (auto system : mSystems)
	{
		if (T* s = dynamic_cast<T*>(system)) // TODO : Find how to not use dynamic_cast
		{
			return s;
		}
	}
	return nullptr;
}

template <typename T>
const T* World::GetSystem() const
{
	static_assert(Traits::IsBaseOf<System, T>::value);
	for (auto system : mSystems)
	{
		if (const T* s = dynamic_cast<const T*>(system)) // TODO : Find how to not use dynamic_cast
		{
			return s;
		}
	}
	return nullptr;
}

template <typename T>
bool World::HasSystem() const
{
	return GetSystem<T>() != nullptr;
}

} // namespace en