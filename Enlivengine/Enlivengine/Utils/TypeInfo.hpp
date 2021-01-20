#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

#include <Enlivengine/Utils/TypeTraits.hpp>
#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/Macros.hpp>
#include <Enlivengine/Utils/String.hpp>
#include <string>

#include <array>
#include <vector>

namespace en
{
	
template <typename T>
struct TypeInfo
{
	using Type = T;
	static constexpr bool IsKnown() { return false; }
	static constexpr const char* GetName() { return "<Unknown>"; }
	//static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } // This has been removed to explicit the fact that it is invalid
	static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(T); }
	static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(T); }
	static constexpr bool IsPrimitive() { return false; }
	static constexpr bool HasCustomSerialization() { return false; }
	static constexpr bool HasCustomEditor() { return false; }
};

} // namespace en

//////////////////////////////////////////////////////////////////
// Define Helpers
//////////////////////////////////////////////////////////////////

#define ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(type) namespace en { \
	template <> \
	struct TypeInfo<type> \
	{ \
		using Type = type; \
		static constexpr bool IsKnown() { return true; } \
		static constexpr const char* GetName() { return #type; } \
		static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } \
		static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(type); } \
		static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(type); } \
		static constexpr bool IsPrimitive() { return true; } \
		static constexpr bool HasCustomSerialization() { return false; } \
		static constexpr bool HasCustomEditor() { return false; } \
	}; } // namespace en

#define ENLIVE_DEFINE_ENUM_INFO(enumName) namespace en { \
	template <> \
	struct TypeInfo<enumName> \
	{ \
		using Type = enumName; \
		static constexpr bool IsKnown() { return true; } \
		static constexpr const char* GetName() { return #enumName; } \
		static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } \
		static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(enumName); } \
		static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(enumName); } \
		static constexpr bool IsPrimitive() { return false; } \
		static constexpr bool HasCustomSerialization() { return false; } \
		static constexpr bool HasCustomEditor() { return false; } \
	}; } // namespace en

#define ENLIVE_DEFINE_TYPE_INFO(type, customSerialization, customEditor) namespace en { \
	template <> \
	struct TypeInfo<type> \
	{ \
		using Type = type; \
		static constexpr bool IsKnown() { return true; } \
		static constexpr const char* GetName() { return #type; } \
		static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } \
		static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(type); } \
		static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(type); } \
		static constexpr bool IsPrimitive() { return false; } \
		static constexpr bool HasCustomSerialization() { return customSerialization; } \
		static constexpr bool HasCustomEditor() { return customEditor; } \
	}; } // namespace en

#define ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(templateType, customSerialization, customEditor) namespace en { \
	template <typename T> \
	struct TypeInfo<templateType<T>> \
	{ \
	private: \
		static constexpr U32 s_stringStorageSize = StringLength(#templateType) + StringLength(TypeInfo<T>::GetName()) + StringLength("<>") + 1; \
		static constexpr ConstexprStringStorage s_stringStorage = ConstexprStringStorage<s_stringStorageSize>(#templateType, "<", TypeInfo<T>::GetName(), ">"); \
	public: \
		using Type = templateType<T>; \
		using ElementType = T; \
		static constexpr bool IsKnown() { return true; } \
		static constexpr const char* GetName() { return s_stringStorage.GetData(); } \
		static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } \
		static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(templateType<T>); } \
		static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(templateType<T>); } \
		static constexpr bool IsPrimitive() { return false; } \
		static constexpr bool HasCustomSerialization() { return customSerialization; } \
		static constexpr bool HasCustomEditor() { return customEditor; } \
	}; } // namespace en


//////////////////////////////////////////////////////////////////
// Primitive types
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(bool)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(char)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::I8)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::U8)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::I16)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::U16)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::I32)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::U32)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::I64)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::U64)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::F32)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(en::F64)
ENLIVE_DEFINE_PRIMITIVE_TYPE_INFO(std::string) // Not technically Primitive but ok

//////////////////////////////////////////////////////////////////
// std
//////////////////////////////////////////////////////////////////

// TODO : Find how to handle the std::array & std::vector cases

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
	static constexpr bool IsPrimitive() { return false; }
	static constexpr bool HasCustomSerialization() { return false; }
	static constexpr bool HasCustomEditor() { return false; }
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(std::vector, false, false)