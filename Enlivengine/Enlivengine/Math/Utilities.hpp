#pragma once

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <cmath>
#include <cstring> // memcpy
#include <limits> // epsilon<T>

// TODO : Real : Float/Double
// TODO : More constexpr + fast functions
// TODO : See https://github.com/pmttavara/pt_math

namespace en
{

class Math
{
public:
	Math() = delete;

	static constexpr F32 EPSILON { 0.0001f };
	static constexpr F32 PI { 3.141592653589793238462643383f };
	static constexpr F32 TWO_PI { 2.0f * PI };
	static constexpr F32 HALF_PI { 0.5f * PI };
	static constexpr F32 INV_PI { 1.0f / PI };
	static constexpr F32 INV_TWO_PI { 1.0f / TWO_PI };
	static constexpr F32 DEG_TO_RAD { PI / 180.0f };
	static constexpr F32 RAD_TO_DEG { 180.0f / PI };

	static inline constexpr F32 DegToRad(F32 value) { return value * DEG_TO_RAD; }
	static inline constexpr F32 RadToDeg(F32 value) { return value * RAD_TO_DEG; }

	static inline F32 Acos(F32 value) { return RadToDeg(std::acos(value)); }
	static inline F32 Asin(F32 value) { return RadToDeg(std::asin(value)); }
	static inline F32 Atan(F32 value) { return RadToDeg(std::atan(value)); }
	static inline F32 Atan2(F32 x, F32 y) { return RadToDeg(std::atan2(y, x)); }
	static inline F32 Cos(F32 value) { return std::cos(DegToRad(value)); }
	static inline F32 Sin(F32 value) { return std::sin(DegToRad(value)); }
	static inline F32 Tan(F32 value) { return std::tan(DegToRad(value)); }

	static inline F32 Sqr(F32 value) { return value * value; }
	static inline F32 Sqrt(F32 value) { return std::sqrt(value); }
	static inline F32 InvSqrt(F32 value) { return 1 / Sqrt(value); }

	static inline F32 Log(F32 value) { return std::log(value); }
	static inline F32 Exp(F32 value) { return std::exp(value); }
	static inline F32 Pow(F32 value, F32 exponent) { return std::pow(value, exponent); }

	static inline F32 Ceil(F32 value) { return std::ceil(value); }
	static inline F32 Floor(F32 value) { return std::floor(value); }

	template <typename T>
	static inline constexpr T Min(T x, T y) { return (x <= y) ? x : y; }
	template <typename T>
	static inline constexpr T Max(T x, T y) { return (x <= y) ? y : x; }
	template <typename T>
	static inline constexpr T Clamp(T value, T valueMin, T valueMax) { assert(valueMin < valueMax); return Max(Min(value, valueMax), valueMin); }
	template <typename T>
	static inline constexpr T Abs(T value) { return (value < 0) ? -value : value; }

	static inline constexpr F32 Lerp(F32 start, F32 end, F32 percent) { return start + percent * (end - start); }
	template <typename T>
	static inline constexpr T Fade(T value) { return value * value * value * (value * (value * 6 - 15) + 10); }

	template <typename T>
	static inline constexpr bool InRange(T value, T valueMin, T valueMax) { return value >= valueMin && value <= valueMax; }
	static inline constexpr bool Equals(F32 x, F32 y, F32 epsilon = EPSILON) { return Abs(x - y) < epsilon; }
	static inline constexpr bool Equals(I32 x, I32 y) { return x == y; }
	static inline constexpr bool Equals(U32 x, U32 y) { return x == y; }
};

} // namespace en