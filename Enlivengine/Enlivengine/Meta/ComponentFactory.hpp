#pragma once

#include <unordered_map>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Meta/ObjectEditor.hpp>
#include <Enlivengine/Meta/DataFile.hpp>

namespace en
{

class ComponentFactory
{
public:
	ComponentFactory() = delete;

	template <typename T>
	static bool IsRegistered();
	static bool IsRegistered(U32 componentHash);

	template <typename T>
	static bool Register();

#ifdef ENLIVE_ENABLE_IMGUI
	using EditorCallback = std::function<bool(Entity&)>;
#endif // ENLIVE_ENABLE_IMGUI
	using AddCallback = std::function<void(Entity&)>;
	using HasCallback = std::function<bool(const Entity&)>;
	using RemoveCallback = std::function<void(Entity&)>;
	using SerializeCallback = std::function<bool(DataFile&, const Entity&)>;
	using DeserializeCallback = std::function<bool(DataFile&, Entity&)>;

	struct ComponentInfo
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

	static const std::unordered_map<U32, ComponentInfo>& GetComponentInfos();

private:
	static std::unordered_map<U32, ComponentInfo> mComponents;
};

template <typename T>
bool ComponentFactory::IsRegistered()
{
	return IsRegistered(TypeInfo<T>::GetHash());
}

template <typename T>
bool ComponentFactory::Register()
{
	static_assert(Meta::IsRegistered<T>());
	static_assert(TypeInfo<T>::IsKnown());
	constexpr U32 hash = TypeInfo<T>::GetHash();
	mComponents[hash].name = TypeInfo<T>::GetName();
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
	mComponents[hash].has = [](const Entity& entity)
	{
		const ENTT_ID_TYPE type[] = { entt::type_info<T>::id() };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
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
			return dataFile.Deserialize(entity.Add<T>(), TypeInfo<T>::GetName());
		}
	};
	return true;
}

} // namespace en