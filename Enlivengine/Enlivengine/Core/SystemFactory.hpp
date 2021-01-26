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

	using EditorCallback = std::function<bool(ObjectEditor& objectEditor, World&)>;
	using AddCallback = std::function<void(World&)>;
	using HasCallback = std::function<bool(World&)>;
	using RemoveCallback = std::function<void(World&)>;
	using SerializeCallback = std::function<bool(Serializer&, World&)>;

	struct SystemInfo
	{
		const char* name;
		EditorCallback editor;
		AddCallback add;
		HasCallback has;
		RemoveCallback remove;
		SerializeCallback serialize;
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
	mSystems[hash].editor = [](ObjectEditor& objectEditor, World& world)
	{
		return GenericEdit(objectEditor, TypeInfo<T>::GetName(), *world.GetSystemManager().GetSystem<T>());
	};
	mSystems[hash].add = [](World& world)
	{
		world.GetSystemManager().CreateSystem<T>();
	};
	mSystems[hash].has = [](World& world)
	{
		return world.GetSystemManager().HasSystem<T>();
	};
	mSystems[hash].remove = [](World& world)
	{
		world.GetSystemManager().RemoveSystem<T>();
	};
	mSystems[hash].serialize = [](Serializer& serializer, World& world)
	{
		return GenericSerialization(serializer, TypeInfo<T>::GetName(), *world.GetSystemManager().GetSystem<T>());
	};
	return true;
}

} // namespace en