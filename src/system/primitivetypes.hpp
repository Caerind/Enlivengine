#pragma once

#include "compilertraits.hpp"

namespace en
{

// 8 bits integer types
using I8 = signed char;
using U8 = unsigned char;

// 16 bits integer types
using I16 = signed short;
using U16 = unsigned short;

// 32 bits integer types
using I32 = signed int;
using U32 = unsigned int;

// 64 bits integer types
#if defined(_MSC_VER)
	using I64 = signed __int64;
	using U64 = unsigned __int64;
#else
	using I64 = signed long long;
	using U64 = unsigned long long;
#endif

// Floating point
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

constexpr I8 I8_Min = -128;
constexpr I8 I8_Max = 127;
constexpr I16 I16_Min = -32768;
constexpr I16 I16_Max = 32767;
constexpr I32 I32_Min = -I32(2147483648); // Workaround as it is recognized as unsigned, there is tests below to ensures this works everywhere
constexpr I32 I32_Max = 2147483647;
constexpr I64 I64_Min = -I64(9223372036854775808); // Workaround as it is recognized as unsigned, there is tests below to ensures this works everywhere
constexpr I64 I64_Max = 9223372036854775807;

// Temporary disable overflow detection
#if defined(ENLIVE_COMPILER_MSVC)
	#pragma warning(disable: 4307)
#endif

static_assert(I8(I8_Max + 1) == I8_Min);
static_assert(I8(I8_Min - 1) == I8_Max);
static_assert(I16(I16_Max + 1) == I16_Min);
static_assert(I16(I16_Min - 1) == I16_Max);
static_assert(I32(I32_Max + 1) == I32_Min);
static_assert(I32(I32_Min - 1) == I32_Max);
static_assert(I64(I64_Max + 1) == I64_Min);
static_assert(I64(I64_Min - 1) == I64_Max); 

// Re-enable overflow detection
#if defined(ENLIVE_COMPILER_MSVC)
	#pragma warning(default: 4307)
#endif

constexpr U8 U8_Min = 0; 
constexpr U8 U8_Max = 255;
constexpr U16 U16_Min = 0;
constexpr U16 U16_Max = 65535;
constexpr U32 U32_Min = 0;
constexpr U32 U32_Max = 4294967295;
constexpr U64 U64_Min = 0;
constexpr U64 U64_Max = 18446744073709551615;

static_assert(U8_Max == U8(-1));
static_assert(U16_Max == U16(-1));
static_assert(U32_Max == U32(-1));
static_assert(U64_Max == U64(-1));

// TODO : Float min/max/epsilon
// TODO : Double min/max/epsilon

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