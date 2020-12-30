#pragma once

#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/Array.hpp>
#include <Enlivengine/Graphics/DebugDraw.hpp>

#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/System.hpp>
#include <Enlivengine/Core/PhysicSystemBase.hpp>

namespace en
{

class World
{
public:
	World(const std::string& name);
	~World();

	EntityManager& GetEntityManager();
	const EntityManager& GetEntityManager() const;

	template <typename T>
	T* CreateSystem();
	template <typename T>
	void RemoveSystem();

	template <typename T>
	T* GetSystem();
	template <typename T>
	const T* GetSystem() const;
	template <typename T>
	bool HasSystem() const;

	PhysicSystemBase* GetPhysicSystem();
	const PhysicSystemBase* GetPhysicSystem() const;
	bool HasPhysicSystem() const;
	
	void Update(Time dt);
	void Render();

	const std::string& GetName() const;
	std::string GetFilename() const;
	static std::string GetWorldFilename(const std::string& worldName);

#ifdef ENLIVE_DEBUG
	void Play();
	void Pause();
	bool IsPlaying() const;

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
	PhysicSystemBase* mPhysicSystem;

	std::string mName;

#ifdef ENLIVE_DEBUG
	bool mPlaying;

	DebugDraw mDebugDraw;

	std::vector<entt::entity> mSelectedEntities;
#endif // ENLIVE_DEBUG
};

template <typename T>
T* World::CreateSystem()
{
	static_assert(Traits::IsBaseOf<System, T>::value);

	if (T* system = GetSystem<T>())
	{
		enLogWarning(LogChannel::Core, "World have too many {}", TypeInfo<T>::GetName());
		return system;
	}
	else
	{
		system = enNew(T, "System");
		if (system != nullptr)
		{
			mSystems.push_back(system);

			system->SetWorld(this);

			if constexpr (Traits::IsBaseOf<PhysicSystemBase, T>::value)
			{
				mPhysicSystem = system;
			}
		}
		return system;
	}
}

template <typename T>
void World::RemoveSystem()
{
	static_assert(Traits::IsBaseOf<System, T>::value);
	const U32 systemCount = static_cast<U32>(mSystems.size());
	for (U32 i = 0; i < systemCount; ++i)
	{
		if (const T* s = dynamic_cast<const T*>(mSystems[i])) // TODO : Find how to not use dynamic_cast
		{
			if constexpr (Traits::IsBaseOf<PhysicSystemBase, T>::value)
			{
				if (mPhysicSystem == mSystems[i])
				{
					mPhysicSystem = nullptr;
				}
			}

			enDelete(System, mSystems[i]);

			mSystems.erase(mSystems.begin() + i);

			return;
		}
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