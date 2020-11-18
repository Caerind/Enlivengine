#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

#include <Enlivengine/Utils/TypeTraits.hpp>
#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/Macros.hpp>

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