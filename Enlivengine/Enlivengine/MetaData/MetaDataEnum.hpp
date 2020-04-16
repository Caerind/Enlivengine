#pragma once

#include <Enlivengine/System/Config.hpp>

#ifdef ENLIVE_ENABLE_METADATA

#include <Enlivengine/MetaData/MetaDataBase.hpp>

namespace en
{

class MetaDataEnumValue
{
public:
    constexpr MetaDataEnumValue() = delete;
    constexpr MetaDataEnumValue(U32 id, const char* name, U32 value, U32 attributes = Attribute_None) : mID(id), mName(name), mValue(value), mAttributes(attributes) {}

    constexpr U32 GetID() const { return mID; }
    constexpr const char* GetName() const { return mName; }
    constexpr U32 GetValue() const { return mValue; }
    constexpr U32 GetAttributes() const { return mAttributes; }

    constexpr bool operator==(const MetaDataEnumValue& other) const { return mID == other.mID; }
    constexpr bool operator!=(const MetaDataEnumValue& other) const { return mID != other.mID; }

private:
    U32 mID;
    const char* mName;
    U32 mValue;
    U32 mAttributes;
};

class MetaDataEnum
{
public:
    constexpr MetaDataEnum() = delete;
    constexpr MetaDataEnum(U32 id, const char* name, const MetaDataEnumValue* values, U32 valueCount, U32 attributes = Attribute_None) : mID(id), mName(name), mValues(values), mValueCount(valueCount), mAttributes(attributes) {}

    constexpr U32 GetID() const { return mID; }
    constexpr const char* GetName() const { return mName; }
    constexpr U32 GetValueCount() const { return mValueCount; }
    constexpr const MetaDataEnumValue& GetValueByIndex(U32 index) const { return mValues[index]; }
    constexpr U32 GetAttributes() const { return mAttributes; }

    constexpr const MetaDataEnumValue& GetValueByValue(U32 value) const 
    { 
        for (U32 i = 0; i < mValueCount; ++i)
        {
            if (mValues[i].GetValue() == value)
            {
                return mValues[i];
            }
        }
        return mValues[0];
    }

    constexpr bool operator==(const MetaDataEnum& other) const { return mID == other.mID; }
    constexpr bool operator!=(const MetaDataEnum& other) const { return mID != other.mID; }

private:
    U32 mID;
    const char* mName;
    const MetaDataEnumValue* mValues;
    U32 mValueCount;
    U32 mAttributes;
};

} // namespace en

#endif // ENLIVE_ENABLE_METADATA