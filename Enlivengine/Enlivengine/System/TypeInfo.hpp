#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/Macros.hpp>
#include <Enlivengine/System/String.hpp> 

// TODO : Define in some other place (along with math stuffs, color, date, ...)
#include <array>
#include <vector>

namespace en
{
	
template <typename T>
struct TypeInfo
{
	using type = T;
	static constexpr bool IsKnown() { return false; }
	static constexpr const char* GetName() { return "<Unknown>"; }
	//static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } // This has been removed to explicit the fact that it is invalid
	static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(T); }
	static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(T); }
};

} // namespace en

#define ENLIVE_DEFINE_TYPE_INFO(templateType) namespace en { \
	template <> \
	struct TypeInfo<templateType> \
	{ \
		using type = templateType; \
		static constexpr bool IsKnown() { return true; } \
		static constexpr const char* GetName() { return #templateType; } \
		static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } \
		static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(type); } \
		static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(type); } \
	}; } // namespace en

#define ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(templateType) namespace en { \
	template <typename T> \
	struct TypeInfo<templateType<T>> \
	{ \
	private: \
		static constexpr U32 s_stringStorageSize = StringLength(#templateType) + StringLength(TypeInfo<T>::GetName()) + StringLength("<>") + 1; \
		static constexpr ConstexprStringStorage s_stringStorage = ConstexprStringStorage<s_stringStorageSize>(#templateType, "<", TypeInfo<T>::GetName(), ">"); \
	public: \
		using type = templateType<T>; \
		using elementType = T; \
		static constexpr bool IsKnown() { return true; } \
		static constexpr const char* GetName() { return s_stringStorage.GetData(); } \
		static constexpr U32 GetHash() { return Hash::SlowHash(GetName()); } \
		static constexpr U32 GetSize() { return ENLIVE_SIZE_OF(templateType<T>); } \
		static constexpr U32 GetAlign() { return ENLIVE_ALIGN_OF(templateType<T>); } \
	}; } // namespace en

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
