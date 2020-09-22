#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/System/Meta.hpp>

#include <Enlivengine/Core/Entity.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <Enlivengine/Core/ObjectEditor.hpp>
#endif // ENLIVE_ENABLE_IMGUI
#include <Enlivengine/Core/DataFile.hpp>

namespace en
{

class ComponentManager
{
public:
	using ComponentTypeID = ENTT_ID_TYPE;

	ComponentManager() = delete;

	static bool IsRegistered(U32 componentHash)
	{
		return mComponents.find(componentHash) != mComponents.end();
	}

	template <typename T>
	static bool IsRegistered()
	{
		return IsRegistered(TypeInfo<T>::GetHash());
	}

	template <typename T>
	static bool Register()
	{
		static_assert(Meta::IsRegistered<T>());
		static_assert(TypeInfo<T>::IsKnown());
		constexpr U32 hash = TypeInfo<T>::GetHash();
		mComponents[hash].name = TypeInfo<T>::GetName();
		mComponents[hash].enttID = entt::type_info<T>::id();
#ifdef ENLIVE_ENABLE_IMGUI
		mComponents[hash].editor = [](Entity& entity)
		{
			T& component = entity.Get<T>();
			return ObjectEditor::ImGuiEditor(component, TypeInfo<T>::GetName());
		};
		mComponents[hash].add = [](Entity& entity)
		{
			entity.Add<T>();
		};
		mComponents[hash].remove = [](Entity& entity)
		{
			entity.Remove<T>();
		};
#endif // ENLIVE_ENABLE_IMGUI
		mComponents[hash].serialize = [](DataFile& dataFile, const Entity& entity)
		{
			return dataFile.Serialize(entity.Get<T>(), TypeInfo<T>::GetName());
		};
		mComponents[hash].deserialize = [](DataFile& dataFile, Entity& entity)
		{
			T& component = entity.Add<T>();
			return dataFile.Deserialize(component, TypeInfo<T>::GetName());
		};
		return true;
	}

#ifdef ENLIVE_ENABLE_IMGUI
	using EditorCallback = std::function<bool(Entity&)>;
	using AddCallback = std::function<void(Entity&)>;
	using RemoveCallback = std::function<void(Entity&)>;
#endif // ENLIVE_ENABLE_IMGUI
	using SerializeCallback = std::function<bool(DataFile&, const Entity&)>;
	using DeserializeCallback = std::function<bool(DataFile&, Entity&)>;

	struct ComponentInfo
	{
		const char* name;
		ComponentTypeID enttID;
#ifdef ENLIVE_ENABLE_IMGUI
		EditorCallback editor;
		AddCallback add;
		RemoveCallback remove;
#endif // ENLIVE_ENABLE_IMGUI
		SerializeCallback serialize;
		DeserializeCallback deserialize;
	};

	static const std::unordered_map<U32, ComponentInfo>& GetComponentInfos() 
	{ 
		return mComponents; 
	}

private:
	static std::unordered_map<U32, ComponentInfo> mComponents;
};

} // namespace en