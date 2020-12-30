#pragma once

#include <unordered_map>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Core/World.hpp>

#include <Enlivengine/Meta/MetaSpecialization.hpp>

#include <Enlivengine/Meta/ObjectEditor.hpp>
#include <Enlivengine/Meta/DataFile.hpp>

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

#ifdef ENLIVE_ENABLE_IMGUI
	using EditorCallback = std::function<bool(World&)>;
#endif // ENLIVE_ENABLE_IMGUI
	using AddCallback = std::function<void(World&)>;
	using HasCallback = std::function<bool(World&)>;
	using RemoveCallback = std::function<void(World&)>;
	using SerializeCallback = std::function<bool(DataFile&, const World&)>;
	using DeserializeCallback = std::function<bool(DataFile&, World&)>;

	struct SystemInfo
	{
		const char* name;
#ifdef ENLIVE_ENABLE_IMGUI
		EditorCallback editor;
#endif // ENLIVE_ENABLE_IMGUI
		AddCallback add;
		HasCallback has;
		RemoveCallback remove;
		SerializeCallback serialize;
		DeserializeCallback deserialize;
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
#ifdef ENLIVE_ENABLE_IMGUI
	mSystems[hash].editor = [](World& world)
	{
		return ObjectEditor::ImGuiEditor(world.GetSystem<T>(), TypeInfo<T>::GetName());
	};
#endif // ENLIVE_ENABLE_IMGUI
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
	mSystems[hash].serialize = [](DataFile& dataFile, const World& world)
	{
		return dataFile.Serialize(world.GetSystem<T>(), TypeInfo<T>::GetName());
	};
	mSystems[hash].deserialize = [](DataFile& dataFile, World& world)
	{
		return dataFile.Deserialize(world.CreateSystem<T>(), TypeInfo<T>::GetName());
	};
	return true;
}

} // namespace en