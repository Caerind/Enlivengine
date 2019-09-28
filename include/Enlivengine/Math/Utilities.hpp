#pragma once

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <cmath>
#include <cstring> // memcpy
#include <limits> // epsilon<T>

// TODO : More constexpr + fast functions
// TODO : See https://github.com/pmttavara/pt_math

namespace en
{

class Math
{
public:
	Math() = delete;

	static constexpr Real EPSILON { 0.0001 };
	static constexpr Real PI { 3.141592653589793238462643383 };
	static constexpr Real TWO_PI { 2 * PI };
	static constexpr Real HALF_PI { 0.5 * PI };
	static constexpr Real INV_PI { 1 / PI };
	static constexpr Real INV_TWO_PI { 1 / TWO_PI };
	static constexpr Real DEG_TO_RAD { PI / 180 };
	static constexpr Real RAD_TO_DEG { 180 / PI };

	static inline constexpr Real DegToRad(Real value) { return value * DEG_TO_RAD; }
	static inline constexpr Real RadToDeg(Real value) { return value * RAD_TO_DEG; }

	static inline Real Acos(Real value) { return RadToDeg(std::acos(value)); }
	static inline Real Asin(Real value) { return RadToDeg(std::asin(value)); }
	static inline Real Atan(Real value) { return RadToDeg(std::atan(value)); }
	static inline Real Atan2(Real x, Real y) { return RadToDeg(std::atan2(y, x)); }
	static inline Real Cos(Real value) { return std::cos(DegToRad(value)); }
	static inline Real Sin(Real value) { return std::sin(DegToRad(value)); }
	static inline Real Tan(Real value) { return std::tan(DegToRad(value)); }

	static inline Real Sqr(Real value) { return value * value; }
	static inline Real Sqrt(Real value) { return std::sqrt(value); }
	static inline Real InvSqrt(Real value) { return 1 / Sqrt(value); }

	static inline Real Log(Real value) { return std::log(value); }
	static inline Real Exp(Real value) { return std::exp(value); }
	static inline Real Pow(Real value, Real exponent) { return std::pow(value, exponent); }

	static inline Real Ceil(Real value) { return std::ceil(value); }
	static inline Real Floor(Real value) { return std::floor(value); }

	template <typename T>
	static inline constexpr T Min(T x, T y) { return (x <= y) ? x : y; }
	template <typename T>
	static inline constexpr T Max(T x, T y) { return (x <= y) ? y : x; }
	template <typename T>
	static inline constexpr T Clamp(T value, T valueMin, T valueMax) { assert(valueMin < valueMax); return Max(Min(value, valueMax), valueMin); }
	template <typename T>
	static inline constexpr T Abs(T value) { return (value < 0) ? -value : value; }

	static inline constexpr Real Lerp(Real start, Real end, Real percent) { return start + percent * (end - start); }
	template <typename T>
	static inline constexpr T Fade(T value) { return value * value * value * (value * (value * 6 - 15) + 10); }

	template <typename T>
	static inline constexpr bool InRange(T value, T valueMin, T valueMax) { return value >= valueMin && value <= valueMax; }
	static inline constexpr bool Equals(Real x, Real y, Real epsilon = EPSILON) { return Abs(x - y) < epsilon; }
};

} // namespace en