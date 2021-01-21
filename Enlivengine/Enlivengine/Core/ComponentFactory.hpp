#pragma once

#include <unordered_map>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Utils/Serializer.hpp>
#include <Enlivengine/Core/Entity.hpp>

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
	using SerializeCallback = std::function<bool(ClassSerializer&, Entity&)>;

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
		ENLIVE_UNUSED(entity);
		enAssert(false);
		return false;
		// TODO : ObjectEditor
		/*
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
		*/
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
	mComponents[hash].serialize = [](ClassSerializer& serializer, Entity& entity)
	{
		if constexpr (Traits::IsEmpty<T>::value)
		{
			if (serializer.BeginClass(TypeInfo<T>::GetName(), TypeInfo<T>::GetHash()))
			{
				return serializer.EndClass();
			}
			else
			{
				return false;
			}
		}
		else
		{
			return GenericSerialization(serializer, TypeInfo<T>::GetName(), entity.Get<T>());
		}
	};
	return true;
}

} // namespace en