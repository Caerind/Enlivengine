#pragma once

#include <Enlivengine/System/Config.hpp>

#ifdef ENLIVE_ENABLE_METADATA

#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/MetaData/MetaDataBase.hpp>

namespace en
{

class MetaDataType;

class MetaDataProperty
{
public:
	constexpr MetaDataProperty() = delete;
	constexpr MetaDataProperty(U32 id, const MetaDataType& type, const char* name, U32 offset, U32 size, U32 typeTraits, U32 elementCount = 1, U32 attributes = Attribute_None) : mID(id), mType(type), mName(name), mOffset(offset), mSize(size), mTypeTraits(typeTraits), mElementCount(elementCount), mAttributes(attributes) {}

	constexpr U32 GetID() const { return mID; }
	constexpr const MetaDataType& GetType() const { return mType; }
	constexpr const char* GetName() const { return mName; }
	constexpr U32 GetOffset() const { return mOffset; }
    constexpr U32 GetSize() const { return mSize; }
	constexpr U32 GetTypeTraits() const { return mTypeTraits; }
	constexpr U32 GetElementCount() const { return mElementCount; }
	constexpr U32 GetElementSize() const { return mSize / mElementCount; }
    constexpr U32 GetAttributes() const { return mAttributes; }

	constexpr bool operator==(const MetaDataProperty& other) const { return mID == other.mID; }
	constexpr bool operator!=(const MetaDataProperty& other) const { return mID != other.mID; }

	constexpr bool HasAttributes() const { return mAttributes != Attribute_None; }
	constexpr bool IsTransient() const { return (mAttributes & Attribute_Transient) != 0; }
	constexpr bool HasCustomSerialization() const { return (mAttributes & Attribute_CustomSerialization) != 0; }

private:
	U32 mID;
    const MetaDataType& mType;
    const char* mName;
	U32 mOffset;
    U32 mSize;
	U32 mTypeTraits;
	U32 mElementCount;
	U32 mAttributes;
};

class MetaDataType
{
public:
	constexpr MetaDataType() = delete;
	constexpr MetaDataType(U32 id, const char* name, U32 size, U32 alignment, U32 traits, const MetaDataType* parent = nullptr, const MetaDataProperty* properties = nullptr, U32 propertyCount = 0, U32 attributes = Attribute_None, const MetaDataEnum* enumType = nullptr, const MetaDataType* const* templateTypes = nullptr, U32 templateTypeCount = 0, U32 templateID = 0) : mID(id), mName(name), mSize(size), mAlignment(alignment), mTraits(traits), mAttributes(attributes), mSignature(id), mParent(parent), mProperties(properties), mPropertyCount(propertyCount), mEnumType(enumType), mTemplateTypes(templateTypes), mTemplateTypeCount(templateTypeCount), mTemplateID(templateID) { mSignature = GenerateSignature(); }

	constexpr U32 GetID() const { return mID; }
	constexpr const char* GetName() const { return mName; }
	constexpr U32 GetSize() const { return mSize; }
	constexpr U32 GetAlignment() const { return mAlignment; }
	constexpr U32 GetTraits() const { return mTraits; }
	constexpr U32 GetAttributes() const { return mAttributes; }
	constexpr U32 GetSignature() const { return mSignature; }

    constexpr const MetaDataType* GetParent() const { return mParent; }

	constexpr U32 GetPropertyCount() const { return mPropertyCount; }
	constexpr const MetaDataProperty& GetPropertyByIndex(U32 index) const { return mProperties[index]; }

	constexpr const MetaDataEnum* GetEnumType() const { return mEnumType; }

	constexpr U32 IsTemplatedClass() const { return mTemplateID != 0; } // TODO : && TemplateTypeCount > 0
	constexpr U32 GetTemplateTypeCount() const { return mTemplateTypeCount; }
	constexpr const MetaDataType& GetTemplateTypeByIndex(U32 index) const { return *mTemplateTypes[index]; }
	constexpr U32 GetTemplateID() const { return mTemplateID; }

	constexpr bool operator==(const MetaDataType& other) const { return mID == other.mID; }
	constexpr bool operator!=(const MetaDataType& other) const { return mID != other.mID; }

	constexpr bool HasAttributes() const { return mAttributes != Attribute_None; }
	constexpr bool IsTransient() const { return (mAttributes & Attribute_Transient) != 0; }
	constexpr bool HasCustomSerialization() const { return (mAttributes & Attribute_CustomSerialization) != 0; }

private:
	U32 mID;
    const char* mName;
	U32 mSize;
	U32 mAlignment;
	U32 mTraits;
	U32 mAttributes;
	U32 mSignature;
    const MetaDataType* mParent;
	const MetaDataProperty* mProperties;
    U32 mPropertyCount;
	const MetaDataEnum* mEnumType;
	const MetaDataType* const* mTemplateTypes;
	U32 mTemplateTypeCount;
	U32 mTemplateID;

private:
	constexpr U32 GenerateSignature() const
	{
		U32 signature = mID;
		signature = Hash::CombineHash(signature, mSize);
		signature = Hash::CombineHash(signature, mAlignment);
		signature = Hash::CombineHash(signature, mTraits);
		signature = Hash::CombineHash(signature, mAttributes);
		if (mParent != nullptr)
		{
			signature = Hash::CombineHash(signature, mParent->GetSignature());
		}
		if (mProperties != nullptr && mPropertyCount > 0)
		{
			for (U32 i = 0; i < mPropertyCount; ++i)
			{
				signature = Hash::CombineHash(signature, mProperties[i].GetType().GetID());
				signature = Hash::CombineHash(signature, mProperties[i].GetType().GetSize());
				signature = Hash::CombineHash(signature, mProperties[i].GetOffset());
				signature = Hash::CombineHash(signature, mProperties[i].GetTypeTraits());
				signature = Hash::CombineHash(signature, mProperties[i].GetAttributes());
			}
        }
		return signature;
	}

public:
	constexpr U32 GetTotalSizeProperties() const
	{
		U32 totalSize = 0;
		for (U32 i = 0; i < mPropertyCount; ++i)
		{
			totalSize += mProperties[i].GetSize();
		}
		return totalSize;
	}
};

#ifdef ENLIVE_ENABLE_METADATA_CHECKING
template <typename T>
constexpr bool CheckMetaDataType(const en::MetaDataType& metaDataType)
{
	en::U32 totalSizeProperties = metaDataType.GetTotalSizeProperties();
	constexpr const bool hasVTable = en::Traits::HasVirtualDestructor<T>::value;
	constexpr const en::U32 voidPtrSize = ENLIVE_SIZE_OF(void*);
	constexpr const en::U32 typeSize = (hasVTable) ? ENLIVE_SIZE_OF(T) - voidPtrSize : ENLIVE_SIZE_OF(T);
	constexpr const en::U32 typeAlign = ENLIVE_ALIGN_OF(T);
	if (totalSizeProperties == 0)
	{
		if (!hasVTable)
		{
			return typeSize == 1 && typeAlign == 1;
		}
		else
		{
			return typeSize == 0 && typeAlign == voidPtrSize;
		}
	}
	else
	{
		constexpr const en::U32 minSize = (typeSize >= typeAlign) ? typeSize - typeAlign : 0;
		return minSize < totalSizeProperties && totalSizeProperties <= typeSize;
	}
}
#else
template <typename T>
constexpr bool CheckMetaDataType(const en::MetaDataType& metaDataType)
{
	return true:
}
#endif // ENLIVE_ENABLE_METATA_CHECKING

#define ENLIVE_META_PRIMITIVE_TYPE_DECL(primitiveType) \
	template <> \
	class MetaData<primitiveType> \
	{ \
	public: \
		using type = primitiveType; \
		static constexpr const en::MetaDataType& Get() { return s_MetaDataType; } \
	private: \
		static constexpr const en::MetaDataType s_MetaDataType = en::MetaDataType(en::TypeName<primitiveType>::hash, en::TypeName<primitiveType>::name, en::TypeSize<primitiveType>::size, en::TypeAlign<primitiveType>::align, en::TypeTraits_Primitive); \
	};

ENLIVE_META_PRIMITIVE_TYPE_DECL(bool)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::I8)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::U8)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::I16)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::U16)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::I32)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::U32)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::I64)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::U64)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::F32)
ENLIVE_META_PRIMITIVE_TYPE_DECL(en::F64)

#undef ENLIVE_META_PRIMITIVE_TYPE_DECL

} // namespace en

#endif // ENLIVE_ENABLE_METADATA
