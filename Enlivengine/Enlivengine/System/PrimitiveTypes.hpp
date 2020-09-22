#pragma once

#include <cstdint>
#include <cfloat>
#include <cstddef>

#include <Enlivengine/Config.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/PlatformTraits.hpp>

// TODO : NumericLimits<T>
// TODO : Bool type ?
// TODO : 16 sized floating point type ?
// TODO : 128 sized int types ?
// TODO : Char types (char + wchar) (char != signed char != unsigned char)
// TODO : Char constants : \b, \t, \n, ... : https://docs.microsoft.com/fr-fr/cpp/c-language/escape-sequences?view=vs-2019
// TODO : More floating point constants : http://www.cplusplus.com/reference/cfloat/

namespace en
{

using I8 = std::int8_t;
using U8 = std::uint8_t;
using I16 = std::int16_t;
using U16 = std::uint16_t;
using I32 = std::int32_t;
using U32 = std::uint32_t;
using I64 = std::int64_t;
using U64 = std::uint64_t;

using F32 = float;
using F64 = double;

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
constexpr F32 F32_Epsilon = FLT_EPSILON;
constexpr F32 F32_Max = FLT_MAX;
constexpr F64 F64_Epsilon = DBL_EPSILON;
constexpr F64 F64_Max = DBL_MAX;

#if defined(ENLIVE_ENABLE_DOUBLE_PRECISION)
	using Real = F64;
	constexpr Real Real_Epsilon = F64_Epsilon;
	constexpr Real Real_Max = F64_Max;
#else
	using Real = F32;
	constexpr Real Real_Epsilon = F32_Epsilon;
	constexpr Real Real_Max = F32_Max;
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

} // namespace en