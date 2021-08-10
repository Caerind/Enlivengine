#pragma once

#include <utility> // std::swap

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

// TODO : constexpr if/where possible
// TODO : Swap for 64 sized int types
// TODO : Swap for 64 sized floating point type

namespace en
{

enum class Endianness
{
	Unknown = -1,
	LittleEndian,
	BigEndian
};

namespace Endian
{
	static constexpr Endianness GetEndianness()
	{
#if defined(ENLIVE_PLATFORM_LITTLE_ENDIAN)
		return Endianness::LittleEndian;
#elif defined(ENLIVE_PLATFORM_BIG_ENDIAN)
		return Endianness::BigEndian;
#else
		return Endianness::Unknown;
#endif
	}

	// TODO : constexpr
	static inline void SwapBytes(void* buffer, U32 size)
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

	static constexpr U16 SwapU16(U16 value)
	{
		return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
	}

	static constexpr I16 SwapI16(I16 value)
	{
		return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
	}

	static constexpr U32 SwapU32(U32 value)
	{
		return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000)) >> 24;
	}

	static constexpr I32 SwapI32(I32 value)
	{
		return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000)) >> 24;
	}

	// TODO : constexpr
	static inline F32 SwapF32(F32 value)
	{
		union
		{
			U32 m_asU32;
			F32 m_asF32;
		} u;
		u.m_asF32 = value;
		u.m_asU32 = SwapU32(u.m_asU32);
		return u.m_asF32;
	}
};

} // namespace en