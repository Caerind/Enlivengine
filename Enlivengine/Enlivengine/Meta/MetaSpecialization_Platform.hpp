#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Platform/Time.hpp>

//////////////////////////////////////////////////////////////////
// en::Time
//////////////////////////////////////////////////////////////////
template <>
struct HasCustomSerialization<en::Time>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Time& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Time>();
			parser.SetValue(en::ToString(object.AsSeconds()));
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Time& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Time>::GetHash());
			std::string valueString;
			parser.GetValue(valueString);
			object = en::Time::Seconds(en::FromString<en::F32>(valueString));
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::Time>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Time& object, const char* name)
	{
		static constexpr std::size_t maxSize = 256;
		static char concatName[maxSize];
#ifdef ENLIVE_COMPILER_MSVC
		strcpy_s(concatName, name);
		strcat_s(concatName, " (s)");
#else
		strcpy(concatName, name);
		strcat(concatName, " (s)");
#endif // ENLIVE_COMPILER_MSVC
		float valueFloat = static_cast<float>(object.AsSeconds());
		if (ImGui::InputFloat(concatName, &valueFloat))
		{
			object = en::Time::Seconds(static_cast<en::F32>(valueFloat));
			return true;
		}
		return false;
	}
};
#endif // ENLIVE_ENABLE_IMGUI