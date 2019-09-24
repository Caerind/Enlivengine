#pragma once

#include "compilertraits.hpp"

#include <cstdint>

namespace en
{

using I8 = std::int8_t;
using U8 = std::uint8_t;
using I16 = std::int16_t;
using U16 = std::uint16_t;
using I32 = std::int32_t;
using U32 = std::uint32_t;
using I64 = std::int64_t;
using U64 = std::int64_t;

using F32 = float;
using F64 = double;
#if defined(ENLIVE_DOUBLE_PRECISION)
	using Real = F64;
#else
	using Real = F32;
#endif

static_assert(sizeof(I8) == 1);
static_assert(sizeof(U8) == 1);
static_assert(sizeof(I16) == 2);
static_assert(sizeof(U16) == 2);
static_assert(sizeof(I32) == 4);
static_assert(sizeof(U32) == 4);
static_assert(sizeof(I64) == 8);
static_assert(sizeof(U64) == 8);
static_assert(sizeof(F32) == 4);
static_assert(sizeof(F64) == 8);

constexpr I8 I8_Min = INT8_MIN;
constexpr I8 I8_Max = INT8_MAX;
constexpr I16 I16_Min = INT16_MIN;
constexpr I16 I16_Max = INT16_MAX;
constexpr I32 I32_Min = INT32_MIN;
constexpr I32 I32_Max = INT32_MAX;
constexpr I64 I64_Min = INT64_MIN;
constexpr I64 I64_Max = INT64_MAX;
constexpr U8 U8_Min = 0;
constexpr U8 U8_Max = UINT8_MAX;
constexpr U16 U16_Min = 0;
constexpr U16 U16_Max = UINT16_MAX;
constexpr U32 U32_Min = 0;
constexpr U32 U32_Max = UINT32_MAX;
constexpr U64 U64_Min = 0;
constexpr U64 U64_Max = UINT64_MAX;
// TODO : Float min/max/epsilon
// TODO : Double min/max/epsilon

// Temporary disable overflow detection
#if defined(ENLIVE_COMPILER_MSVC)
	#pragma warning(disable: 4307)
#elif defined(ENLIVE_COMPILER_GCC)

#endif

static_assert(I8(I8_Max + 1) == I8_Min);
static_assert(I8(I8_Min - 1) == I8_Max);
static_assert(I16(I16_Max + 1) == I16_Min);
static_assert(I16(I16_Min - 1) == I16_Max);
static_assert(I32(I32_Max + 1) == I32_Min);
static_assert(I32(I32_Min - 1) == I32_Max);
static_assert(I64(I64_Max + 1) == I64_Min);
static_assert(I64(I64_Min - 1) == I64_Max);
static_assert(U8_Max == U8(-1));
static_assert(U16_Max == U16(-1));
static_assert(U32_Max == U32(-1));
static_assert(U64_Max == U64(-1));

// Re-enable overflow detection
#if defined(ENLIVE_COMPILER_MSVC)
	#pragma warning(default: 4307)
#elif defined(ENLIVE_COMPILER_GCC)
#endif

union U32F32
{
	U32 m_asU32;
	F32 m_asF32;
};

inline U32 ConvertToU32Bytes(F32 value) // TODO : constexpr
{
	U32F32 u;
	u.m_asF32 = value;
	return u.m_asU32;
}

inline F32 ConvertToF32Bytes(U32 value) // TODO : constexpr
{
	U32F32 u;
	u.m_asU32 = value;
	return u.m_asF32;
}

} // namespace en