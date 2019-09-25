#pragma once

#include <utility> // std::swap

#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

enum class Endianness
{
	Unknown = -1,
	LittleEndian,
	BigEndian
};

inline constexpr Endianness GetPlatformEndianness()
{
	#if defined(ENLIVE_PLATFORM_LITTLE_ENDIAN)
		return Endianness::LittleEndian;
	#elif defined(ENLIVE_PLATFORM_BIG_ENDIAN)
		return Endianness::BigEndian;
	#else
		return Endianness::Unknown;
	#endif
}

inline void swapBytes(void* buffer, U32 size) // TODO : constexpr
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

inline constexpr U16 swapU16(U16 value)
{
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

inline constexpr I16 swapI16(I16 value)
{
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

inline constexpr U32 swapU32(U32 value)
{
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000)) >> 24;
}

inline constexpr I32 swapI32(I32 value)
{
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000)) >> 24;
}

inline F32 swapF32(F32 value) // TODO : constexpr
{
	U32F32 u;
	u.m_asF32 = value;
	u.m_asU32 = swapU32(u.m_asU32);
	return u.m_asF32;
}

} // namespace en