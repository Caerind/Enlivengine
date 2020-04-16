#pragma once

#include <Enlivengine/MetaData/MetaDataEnum.hpp>
#include <Enlivengine/MetaData/MetaDataType.hpp>

#ifdef ENLIVE_ENABLE_METADATA

// Enums macros
#define ENLIVE_META_ENUM(enumType) ENLIVE_META_ENUM_AS(enumType, en::U32)
#define ENLIVE_META_ENUM_AS(enumType, underlyingType) enum class enumType : underlyingType
#define ENLIVE_META_ENUM_DEF(enumType) \
	ENLIVE_DEFINE_TYPE_TRAITS_NAME(enumType) \
	template <> \
	class MetaData<enumType> \
	{ \
	public: \
		using type = enumType; \
		static constexpr const en::MetaDataEnum& GetEnum() { return s_MetaDataEnum; } \
		static constexpr const en::MetaDataType& Get() { return s_MetaData; } \
	private: \
		static constexpr const en::MetaDataEnumValue s_MetaDataValues[] = {

#define ENLIVE_META_ENUM_VALUE(valueName) en::MetaDataEnumValue(en::Hash::CRC32(#valueName), #valueName, static_cast<en::U32>(type::valueName))

#define ENLIVE_META_ENUM_DEF_END() ENLIVE_META_ENUM_DEF_END_ATTR(en::Attribute_None)
#define ENLIVE_META_ENUM_DEF_END_ATTR(attributes) \
		};\
		static constexpr const en::MetaDataEnum s_MetaDataEnum = en::MetaDataEnum(en::TypeName<type>::hash, en::TypeName<type>::name, s_MetaDataValues, ENLIVE_ARRAY_SIZE(s_MetaDataValues)); \
		static constexpr const en::MetaDataType s_MetaData = en::MetaDataType(en::TypeName<type>::hash, en::TypeName<type>::name, en::TypeSize<type>::size, en::TypeAlign<type>::align, en::TypeTraits_Enum, nullptr, nullptr, 0, attributes, &s_MetaDataEnum); \
	};

// Class macros
#define ENLIVE_META_CLASS_DECL(classType) \
	public: \
		template <typename AnyTypeThatHasMetaData> friend class en::MetaData;

#define ENLIVE_META_CLASS_DEF(classType) \
	ENLIVE_DEFINE_TYPE_TRAITS_NAME(classType) \
	template <> \
	class MetaData<classType> \
	{ \
	public: \
		using type = classType; \
		static constexpr const en::MetaDataType& Get() { return s_MetaData; } \
	private: \
		static constexpr const en::MetaDataProperty s_MetaDataProperties[] = {

#define ENLIVE_META_CLASS_DEF_TEMPLATE(templateBase) \
	ENLIVE_DEFINE_TYPE_TRAITS_NAME_TEMPLATE(templateBase) \
	template <typename T> \
	class MetaData<templateBase<T>> \
	{ \
	public: \
		using type = templateBase<T>; \
		static constexpr const en::MetaDataType& Get() { return s_MetaData; } \
	private: \
		static constexpr const en::MetaDataProperty s_MetaDataProperties[] = {

#define ENLIVE_META_CLASS_PROPERTY(propertyType, propertyName) ENLIVE_META_CLASS_PROPERTY_TRAITS_ATTR(propertyType, propertyName, en::TypeTraits_None, en::Attribute_None)
#define ENLIVE_META_CLASS_PROPERTY_ATTR(propertyType, propertyName, attributes) ENLIVE_META_CLASS_PROPERTY_TRAITS_ATTR(propertyType, propertyName, en::TypeTraits_None, attributes)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS(propertyType, propertyName, traits) ENLIVE_META_CLASS_PROPERTY_TRAITS_ATTR(propertyType, propertyName, traits, en::Attribute_None)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS_ATTR(propertyType, propertyName, traits, attributes) en::MetaDataProperty(en::Hash::CRC32(#propertyName), en::MetaData<propertyType>::Get(), #propertyName, ENLIVE_OFFSET_OF(type, propertyName), ENLIVE_SIZE_OF_MEMBER(type, propertyName), en::MetaData<propertyType>::Get().GetTraits() | traits, attributes)
#define ENLIVE_META_CLASS_PROPERTY_ARRAY(propertyType, propertyName, elementCount) ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY_ATTR(propertyType, propertyName, en::TypeTraits_None, elementCount, en::Attribute_None)
#define ENLIVE_META_CLASS_PROPERTY_ARRAY_ATTR(propertyType, propertyName, elementCount, attributes) ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY_ATTR(propertyType, propertyName, en::TypeTraits_None, elementCount, attributes)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY(propertyType, propertyName, traits, elementCount) ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY_ATTR(propertyType, propertyName, traits, elementCount, en::Attribute_None)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY_ATTR(propertyType, propertyName, traits, elementCount, attributes) en::MetaDataProperty(en::Hash::CRC32(#propertyName), en::MetaData<propertyType>::Get(), #propertyName, ENLIVE_OFFSET_OF(type, propertyName), ENLIVE_SIZE_OF_MEMBER(type, propertyName), en::MetaData<propertyType>::Get().GetTraits() | en::TypeTraits_Array | traits, elementCount, attributes)

#define ENLIVE_META_CLASS_DEF_END() ENLIVE_META_CLASS_DEF_END_EX_ATTR(nullptr, en::Attribute_None)
#define ENLIVE_META_CLASS_DEF_END_ATTR(attributes) ENLIVE_META_CLASS_DEF_END_EX_ATTR(nullptr, attributes)
#define ENLIVE_META_CLASS_DEF_END_EX(parentMetaDataTypePtr) ENLIVE_META_CLASS_DEF_END_EX_ATTR(parentMetaDataTypePtr, en::Attribute_None)
#define ENLIVE_META_CLASS_DEF_END_EX_ATTR(parentMetaDataTypePtr, attributes) \
		}; \
		static constexpr const en::MetaDataType s_MetaData = en::MetaDataType(en::TypeName<type>::hash, en::TypeName<type>::name, en::TypeSize<type>::size, en::TypeAlign<type>::align, en::TypeTraits_Class, parentMetaDataTypePtr, s_MetaDataProperties, ENLIVE_ARRAY_SIZE(s_MetaDataProperties), attributes); \
		static_assert(CheckMetaDataType<type>(s_MetaData), "Invalid meta data class layout"); \
	};

#define ENLIVE_META_CLASS_DEF_END_TEMPLATE(templateBase) ENLIVE_META_CLASS_DEF_END_EX_ATTR_TEMPLATE(nullptr, en::Attribute_None, templateBase)
#define ENLIVE_META_CLASS_DEF_END_ATTR_TEMPLATE(attributes, templateBase) ENLIVE_META_CLASS_DEF_END_EX_ATTR_TEMPLATE(nullptr, attributes, templateBase)
#define ENLIVE_META_CLASS_DEF_END_EX_TEMPLATE(parentMetaDataTypePtr, templateBase) ENLIVE_META_CLASS_DEF_END_EX_ATTR_TEMPLATE(parentMetaDataTypePtr, en::Attribute_None, templateBase)
#define ENLIVE_META_CLASS_DEF_END_EX_ATTR_TEMPLATE(parentMetaDataTypePtr, attributes, templateBase) \
		}; \
		static constexpr const en::MetaDataType s_MetaData = en::MetaDataType(en::TypeName<type>::hash, en::TypeName<type>::name, en::TypeSize<type>::size, en::TypeAlign<type>::align, en::TypeTraits_Class, parentMetaDataTypePtr, s_MetaDataProperties, ENLIVE_ARRAY_SIZE(s_MetaDataProperties), attributes, nullptr, nullptr, 0, en::Hash::CRC32(#templateBase)); \
		static_assert(CheckMetaDataType<type>(s_MetaData), "Invalid meta data class layout"); \
	};

// Utils macros
#define ENLIVE_METADATA_ONLY(code) code
#define ENLIVE_METADATA_COMMA() ,

#else // ENLIVE_ENABLE_METADATA

// Enum macros
#define ENLIVE_META_ENUM(enumType) ENLIVE_META_ENUM_AS(enumType, en::U32)
#define ENLIVE_META_ENUM_AS(enumType, underlyingType) enum class enumType : underlyingType
#define ENLIVE_META_ENUM_DEF(enumType)
#define ENLIVE_META_ENUM_VALUE(valueName)
#define ENLIVE_META_ENUM_DEF_END()
#define ENLIVE_META_ENUM_DEF_END_ATTR(attributes)

// Class macros
#define ENLIVE_META_CLASS_DECL(classType)
#define ENLIVE_META_CLASS_DEF(classType)
#define ENLIVE_META_CLASS_DEF_TEMPLATE(templateBase)
#define ENLIVE_META_CLASS_PROPERTY(propertyType, propertyName)
#define ENLIVE_META_CLASS_PROPERTY_ATTR(propertyType, propertyName, attributes)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS(propertyType, propertyName, traits)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS_ATTR(propertyType, propertyName, traits, attributes)
#define ENLIVE_META_CLASS_PROPERTY_ARRAY(propertyType, propertyName, elementCount)
#define ENLIVE_META_CLASS_PROPERTY_ARRAY_ATTR(propertyType, propertyName, elementCount, attributes)
#define ENLIVE_META_CLASS_PROPERTY_TRAITS_ARRAY(propertyType, propertyName, traits, elementCount)
#define ENLVIE_META_CLASS_PROPERTY_TRAITS_ARRAY_ATTR(propertyType, propertyName, traits, elementCount, attributes)
#define ENLIVE_META_CLASS_DEF_END()
#define ENLIVE_META_CLASS_DEF_END_ATTR(attributes)
#define ENLIVE_META_CMASS_DEF_END_EX(parentMetaDataTypePtr)
#define ENLIVE_META_CLASS_DEF_END_EX_ATTR(parentMetaDataTypePtr, attributes)
#define ENLIVE_META_CLASS_DEF_END_TEMPLATE(templateBase)
#define ENLIVE_META_CLASS_DEF_END_ATTR_TEMPLATE(attributes, templateBase)
#define ENLIVE_META_CLASS_DEF_END_EX_TEMPLATE(parentMetaDataPtr, templateBase)
#define ENLIVE_META_CLASS_DEF_END_EX_ATTR_TEMPLATE(parentMetaDataPtr, attributes, templateBase)

// Utils macros
#define ENLIVE_METADATA_ONLY(code)
#define ENLIVE_METADATA_COMMA()

#endif // ENLIVE_ENABLE_METADATA
