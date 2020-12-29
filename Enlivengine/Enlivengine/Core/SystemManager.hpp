#pragma once

#include <Enlivengine/Utils/TypeInfo.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

class SystemManager
{
public:
	SystemManager() = delete;

	static bool IsRegistered(U32 systemHash)
	{
		return mSystems.find(systemHash) != mSystems.end();
	}

	template <typename T>
	static bool IsRegistered()
	{
		return IsRegistered(TypeInfo<T>::GetHash());
	}

	template <typename T>
	static bool Register()
	{
		static_assert(TypeInfo<T>::IsKnown());
		constexpr U32 hash = TypeInfo<T>::GetHash();
		mSystems[hash].name = TypeInfo<T>::GetName();
		mSystems[hash].add = [](World& world)
		{
			world.CreateSystem<T>();
		};
		mSystems[hash].has = [](World& world)
		{
			return world.HasSystem<T>();
		};
		mSystems[hash].remove = [](World& world)
		{
			world.RemoveSystem<T>();
		};
		return true;
	}

	using AddCallback = std::function<void(World&)>;
	using HasCallback = std::function<bool(World&)>;
	using RemoveCallback = std::function<void(World&)>;

	struct SystemInfo
	{
		const char* name;
		AddCallback add;
		HasCallback has;
		RemoveCallback remove;
	};

	static const std::unordered_map<U32, SystemInfo>& GetSystemInfos()
	{ 
		return mSystems; 
	}

private:
	static std::unordered_map<U32, SystemInfo> mSystems;
};

} // namespace en