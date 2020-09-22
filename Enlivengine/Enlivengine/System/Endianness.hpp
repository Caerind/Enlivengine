#pragma once

#include <utility> // std::swap

#include <Enlivengine/System/PrimitiveTypes.hpp>

// TODO : constexpr if/where possible
// TODO : EndianSwap for 64 sized int types
// TODO : EndianSwap for 64 sized floating point type

namespace en
{

enum class Endianness
{
	Unknown = -1,
	LittleEndian,
	BigEndian
};

constexpr Endianness GetPlatformEndianness()
{
	#if defined(ENLIVE_PLATFORM_LITTLE_ENDIAN)
		return Endianness::LittleEndian;
	#elif defined(ENLIVE_PLATFORM_BIG_ENDIAN)
		return Endianness::BigEndian;
	#else
		return Endianness::Unknown;
	#endif
}

// TODO : C++20 : std::swap will be constexpr
inline void EndianSwapBytes(void* buffer, U32 size)
{
	if (buffer == nullptr || size == 0)
		return;
	U8* bytes = static_cast<U8*>(buffer);
	U32 i = 0;
	U32 j = size - 1;
	while (i < j)
	{
		std::swap(bytes[i++], bytes[j--]);
	}
}

constexpr U16 EndianSwapU16(U16 value)
{
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

constexpr I16 EndianSwapI16(I16 value)
{
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

constexpr U32 EndianSwapU32(U32 value)
{
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000)) >> 24;
}

constexpr I32 EndianSwapI32(I32 value)
{
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000)) >> 24;
}

// TODO : constexpr
inline F32 EndianSwapF32(F32 value) 
{
	union 
	{
		U32 m_asU32;
		F32 m_asF32;
	} u;
	u.m_asF32 = value;
	u.m_asU32 = EndianSwapU32(u.m_asU32);
	return u.m_asF32;
}

} // namespace en