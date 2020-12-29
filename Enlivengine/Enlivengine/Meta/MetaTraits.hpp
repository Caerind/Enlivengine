#pragma once

#include <Enlivengine/Config.hpp>

// TODO : Namespace en::Traits ?

template <typename T>
struct HasCustomSerialization
{
	static constexpr bool value = false;
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor
{
	static constexpr bool value = false;
};
#endif // ENLIVE_ENABLE_IMGUI

// GetName
#define ENLIVE_META_CLASS_VIRTUAL_NAME_DEFINITION() \
	virtual const char* GetName() const;

#define ENLIVE_META_CLASS_VIRTUAL_NAME_DECLARATION(className) \
	const char* className::GetName() const \
	{ \
		 return TypeInfo<className>::GetName(); \
	}

// Custom ImGui Editor
#ifdef ENLIVE_ENABLE_IMGUI

#define ENLIVE_META_CLASS_VIRTUAL_IMGUI_EDITOR_DEFINITION() \
	virtual bool ImGuiEditor(const char* name);

#define ENLIVE_META_CLASS_VIRTUAL_IMGUI_EDITOR_DECLARATION(className) \
	bool className::ImGuiEditor(const char* name) \
	{ \
		 return en::ObjectEditor::ImGuiEditor_Registered(*this, name); \
	}
	
#else
	
#define ENLIVE_META_CLASS_VIRTUAL_IMGUI_EDITOR_DEFINITION()

#define ENLIVE_META_CLASS_VIRTUAL_IMGUI_EDITOR_DECLARATION(className)

#endif // ENLIVE_ENABLE_IMGUI

// Custom Serialization
#define ENLIVE_META_CLASS_VIRTUAL_SERIALIZATION_DEFINITION() \
	virtual bool Serialize(en::DataFile& dataFile, const char* name) const; \
	virtual bool Deserialize(en::DataFile& dataFile, const char* name); \

#define ENLIVE_META_CLASS_VIRTUAL_SERIALIZATION_DECLARATION(className) \
	bool className::Serialize(en::DataFile& dataFile, const char* name) const \
	{ \
		 return dataFile.Serialize_Registered(*this, name); \
	} \
	bool className::Deserialize(en::DataFile& dataFile, const char* name) \
	{ \
		 return dataFile.Deserialize_Registered(*this, name); \
	}