#pragma once

#include <Enlivengine/Core/System.hpp>
#include <Enlivengine/Core/PhysicSystemBase.hpp>
#include <Enlivengine/Core/RenderSystemBase.hpp>

namespace en
{

class SystemManager
{
public:
	SystemManager(World& world);
	~SystemManager();

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

	bool HasPhysicSystem() const;
	PhysicSystemBase* GetPhysicSystem();
	const PhysicSystemBase* GetPhysicSystem() const;

	bool HasRenderSystem() const;
	RenderSystemBase* GetRenderSystem();
	const RenderSystemBase* GetRenderSystem() const;

	void UpdatePhysic();
	void Update();
	void Render();

#ifdef ENLIVE_TOOL
	void UpdateTool();
#endif // ENLIVE_TOOL

	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	World& mWorld;
	Array<System*> mSystems;
	PhysicSystemBase* mPhysicSystem;
	RenderSystemBase* mRenderSystem;
};

template <typename T>
T* SystemManager::CreateSystem()
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
			mSystems.Add(system);

			system->SetWorld(&mWorld);

			if constexpr (Traits::IsBaseOf<PhysicSystemBase, T>::value)
			{
				mPhysicSystem = system;
			}

			if constexpr (Traits::IsBaseOf<RenderSystemBase, T>::value)
			{
				mRenderSystem = system;
			}
		}
		return system;
	}
}

template <typename T>
void SystemManager::RemoveSystem()
{
	static_assert(Traits::IsBaseOf<System, T>::value);
	const U32 systemCount = static_cast<U32>(mSystems.Size());
	for (U32 i = 0; i < systemCount; ++i)
	{
		if (T* s = dynamic_cast<T*>(mSystems[i])) // TODO : Find how to not use dynamic_cast
		{
			if constexpr (Traits::IsBaseOf<PhysicSystemBase, T>::value)
			{
				mPhysicSystem = nullptr;
			}
			
			if constexpr (Traits::IsBaseOf<RenderSystemBase, T>::value)
			{
				mRenderSystem = nullptr;
			}

			enDelete(System, mSystems[i]);

			mSystems.RemoveAtIndex(i);

			return;
		}
	}
}

template <typename T>
T* SystemManager::GetSystem()
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
const T* SystemManager::GetSystem() const
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
bool SystemManager::HasSystem() const
{
	return GetSystem<T>() != nullptr;
}

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::SystemManager, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()