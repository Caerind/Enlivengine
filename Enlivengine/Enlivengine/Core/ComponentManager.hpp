#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>
#include <Enlivengine/Meta/DataFile.hpp>

#include <Enlivengine/Core/Entity.hpp>

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
			if constexpr (Traits::IsEmpty<T>::value)
			{
				ImGui::Text("%s", TypeInfo<T>::GetName());
				return false;
			}
			else
			{
				T& component = entity.Get<T>();
				return ObjectEditor::ImGuiEditor(component, TypeInfo<T>::GetName());
			}
		};
#endif // ENLIVE_ENABLE_IMGUI
		mComponents[hash].add = [](Entity& entity)
		{
			entity.Add<T>();
		};
		mComponents[hash].remove = [](Entity& entity)
		{
			entity.Remove<T>();
		};
		mComponents[hash].serialize = [](DataFile& dataFile, const Entity& entity)
		{
			if constexpr (Traits::IsEmpty<T>::value)
			{
				return true;
			}
			else
			{
				return dataFile.Serialize(entity.Get<T>(), TypeInfo<T>::GetName());
			}
		};
		mComponents[hash].deserialize = [](DataFile& dataFile, Entity& entity)
		{
			if constexpr (Traits::IsEmpty<T>::value)
			{
				return true;
			}
			else
			{
				T& component = entity.Add<T>();
				return dataFile.Deserialize(component, TypeInfo<T>::GetName());
			}
		};
		return true;
	}

#ifdef ENLIVE_ENABLE_IMGUI
	using EditorCallback = std::function<bool(Entity&)>;
#endif // ENLIVE_ENABLE_IMGUI
	using AddCallback = std::function<void(Entity&)>;
	using RemoveCallback = std::function<void(Entity&)>;
	using SerializeCallback = std::function<bool(DataFile&, const Entity&)>;
	using DeserializeCallback = std::function<bool(DataFile&, Entity&)>;

	struct ComponentInfo
	{
		const char* name;
		ComponentTypeID enttID;
#ifdef ENLIVE_ENABLE_IMGUI
		EditorCallback editor;
#endif // ENLIVE_ENABLE_IMGUI
		AddCallback add;
		RemoveCallback remove;
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