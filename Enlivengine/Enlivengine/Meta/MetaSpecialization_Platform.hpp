#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Platform/Time.hpp>
#include <string>
#include <array> // TypeInfo for std::array<T>
#include <vector> // TypeInfo for std::vector<T>

//////////////////////////////////////////////////////////////////
// Primitive types
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(bool)
ENLIVE_DEFINE_TYPE_INFO(char)
ENLIVE_DEFINE_TYPE_INFO(en::I8)
ENLIVE_DEFINE_TYPE_INFO(en::U8)
ENLIVE_DEFINE_TYPE_INFO(en::I16)
ENLIVE_DEFINE_TYPE_INFO(en::U16)
ENLIVE_DEFINE_TYPE_INFO(en::I32)
ENLIVE_DEFINE_TYPE_INFO(en::U32)
ENLIVE_DEFINE_TYPE_INFO(en::I64)
ENLIVE_DEFINE_TYPE_INFO(en::U64)
ENLIVE_DEFINE_TYPE_INFO(en::F32)
ENLIVE_DEFINE_TYPE_INFO(en::F64)

//////////////////////////////////////////////////////////////////
// std
//////////////////////////////////////////////////////////////////

ENLIVE_DEFINE_TYPE_INFO(std::string)

namespace en
{

// std::array
template <typename T, std::size_t N>
struct TypeInfo<std::array<T, N>>
{
private:
	static constexpr const char* nString = ConstexprIntToString<N>::value;
	static constexpr U32 s_stringStorageSize = StringLength("std::array") + StringLength(TypeInfo<T>::GetName()) + StringLength("<,>") + StringLength(nString) + 1;
	static constexpr ConstexprStringStorage s_stringStorage = ConstexprStringStorage<s_stringStorageSize>("std::array<", TypeInfo<T>::GetName(), ",", nString, ">");
public:
	using elementType = T;
	static constexpr bool IsKnown() { return true; }
	static constexpr const char* GetName() { return s_stringStorage.GetData(); }
	static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); }
	static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(T) * N; }
	static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(T); }
};

// std::vector
template <class T>
struct TypeInfo<std::vector<T>>
{
private:
	static constexpr U32 s_stringStorageSize = StringLength("std::vector") + StringLength(TypeInfo<T>::GetName()) + StringLength("<>") + 1;
	static constexpr ConstexprStringStorage s_stringStorage = ConstexprStringStorage<s_stringStorageSize>("std::vector<", TypeInfo<T>::GetName(), ">");
public:
	using type = std::vector<T>;
	using elementType = T;
	static constexpr bool IsKnown() { return true; }
	static constexpr const char* GetName() { return s_stringStorage.GetData(); }
	static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); }
	static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(std::vector<T>); }
	static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(std::vector<T>); }
};

} // namespace en

//////////////////////////////////////////////////////////////////
// en::Time
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(en::Time)

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