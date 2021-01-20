#pragma once

#include <unordered_map>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

class SystemFactory
{
public:
	SystemFactory() = delete;

	template <typename T>
	static bool IsRegistered();
	static bool IsRegistered(U32 systemHash);

	template <typename T>
	static bool Register();

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

	static const std::unordered_map<U32, SystemInfo>& GetSystemInfos();

private:
	static std::unordered_map<U32, SystemInfo> mSystems;
};

template <typename T>
bool SystemFactory::IsRegistered()
{
	return IsRegistered(TypeInfo<T>::GetHash());
}

template <typename T>
bool SystemFactory::Register()
{
	static_assert(Meta::IsRegistered<T>());
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

} // namespace en