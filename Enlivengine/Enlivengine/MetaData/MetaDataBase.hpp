#pragma once

#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/TypeTraits.hpp>

#ifdef ENLIVE_ENABLE_METADATA

namespace en
{

// MetaData Attributes
static constexpr U32 Attribute_None = 0;
static constexpr U32 Attribute_Transient = (1 << 0);
static constexpr U32 Attribute_CustomSerialization = (1 << 1);

// MetaData Traits
static constexpr U32 TypeTraits_None = 0;
static constexpr U32 TypeTraits_Null = (1 << 0); // void, nullptr_t
static constexpr U32 TypeTraits_Primitive = (1 << 1); // int, float, char, bool, double, long, +/- unsigned, short, ...
static constexpr U32 TypeTraits_Pointer = (1 << 2); // *
static constexpr U32 TypeTraits_Reference = (1 << 3); // &
static constexpr U32 TypeTraits_Enum = (1 << 4); // enum (class)
static constexpr U32 TypeTraits_Class = (1 << 5); // class, struct
static constexpr U32 TypeTraits_Array = (1 << 6); // T myArray[x]
// TODO : static constexpr U32 TypeTraits_ArrayPtr = (1 << 7); // T (*myArray)[]
// TODO : static constexpr U32 TypeTraits_Const = (1 << 8); // const
// TODO : static constexpr U32 TypeTraits_Union = (1 << 9); // union

// Unspecialized class don't store data
template <typename T>
class MetaData
{
	//static constexpr MetaDataType meta = ...
};

} // namespace en

#endif // ENLIVE_ENABLE_METADATA