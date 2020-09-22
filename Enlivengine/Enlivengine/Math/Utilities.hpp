#pragma once

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Meta.hpp>

#include <cmath>
#include <cstring> // memcpy
#include <limits> // epsilon<T>
#include <algorithm> // std::swap

// TODO : Real : Float/Double
// TODO : More constexpr + fast functions
// TODO : See https://github.com/pmttavara/pt_math
// TODO : See https://github.com/kthohr/gcem

namespace en
{

class Math
{
public:
	Math() = delete;

	static constexpr F32 Epsilon { 0.0001f };
	static constexpr F32 Pi { 3.141592653589793238462643383f };
	static constexpr F32 TwoPi { 2.0f * Pi };
	static constexpr F32 HalfPi { 0.5f * Pi };
	static constexpr F32 InvPi { 1.0f / Pi };
	static constexpr F32 InvTwoPi { 1.0f / TwoPi };
	static constexpr F32 kDegToRad { Pi / 180.0f };
	static constexpr F32 kRadToDeg { 180.0f / Pi };

	static constexpr F32 DegToRad(F32 value) { return value * kDegToRad; }
	static constexpr F32 RadToDeg(F32 value) { return value * kRadToDeg; }

	static constexpr F32 Cos(F32 value)
	{
		bool negative = false;
		value = AngleMagnitude(value);
		if (value > 270.0f)
		{
			value = 360.0f - value;
		}
		else if (value > 180.0f)
		{
			value -= 180.0f;
			negative = true;
		}
		else if (value > 90.0f)
		{
			value = 180.0f - value;
			negative = true;
		}
		value = DegToRad(value);
		const F32 x2 = value * value;
		const F32 x4 = x2 * x2;
		const F32 x6 = x4 * x2;
		const F32 x8 = x6 * x2;
		const F32 x10 = x8 * x2;
		value = (1.0f - 0.4999999963f * x2 + 0.0416666418f * x4 - 0.0013888397f * x6 + 0.0000247609f * x8 - 0.0000002605f * x10);
		if (negative)
		{
			value *= -1.0f;
		}
		return value;
	}
	static constexpr F32 Sin(F32 value)
	{
		bool negative = false;
		value = AngleMagnitude(value);
		if (value > 270.0f)
		{
			value = 360.0f - value;
			negative = true;
		}
		else if (value > 180.0f)
		{
			value -= 180.0f;
			negative = true;
		}
		else if (value > 90.0f)
		{
			value = 180.0f - value;
		}
		value = DegToRad(value);
		const F32 x2 = value * value;
		const F32 x4 = x2 * x2;
		const F32 x6 = x4 * x2;
		const F32 x8 = x6 * x2;
		const F32 x10 = x8 * x2;
		value *= (1.0f - 0.1666666664f * x2 + 0.0083333315f * x4 - 0.0001984090f * x6 + 0.0000027526f * x8 - 0.0000000239f * x10);
		if (negative)
		{
			value *= -1.0f;
		}
		return value;
	}
	static constexpr F32 Tan(F32 value)
	{
		// TODO : This can be replaced with a Taylor development
		return Sin(value) / Cos(value);
	}

	static inline F32 Acos(F32 value) { return RadToDeg(std::acos(value)); }
	static inline F32 Asin(F32 value) { return RadToDeg(std::asin(value)); }
	static inline F32 Atan(F32 value) { return RadToDeg(std::atan(value)); }
	static inline F32 Atan2(F32 x, F32 y) { return RadToDeg(std::atan2(y, x)); }

	static constexpr F32 AngleMagnitude(F32 value) 
	{ 
		constexpr F32 magnitude = 360.0f;
		while (value >= magnitude) 
		{ 
			value -= magnitude;
		}; 
		while (value < 0.0f) 
		{ 
			value += magnitude; 
		}; 
		return value; 
	}
	static constexpr F32 AngleBetween(F32 a, F32 b) { const F32 x = Abs(AngleMagnitude(a) - AngleMagnitude(b)); return (x < 180.0f) ? x : 360.0f - x; };
	static constexpr F32 AngleOpposite(F32 value) { return AngleMagnitude(value - 180.0f); }

	template <typename T>
	static constexpr T Sqr(T value) { return value * value; }
	static constexpr F32 Sqrt(F32 value) 
	{
		I32 i = *(I32*)&value;
		i = (1 << 29) + (i >> 1) - (1 << 22);
		F32 f = *(F32*)&i;
		f = f + value / f;
		f = 0.25f * f + value / f;
		return f;
	}
	static constexpr F32 InvSqrt(F32 value) 
	{
		// TODO : This may be improved
		return 1.0f / Sqrt(value);
	}

	static inline F32 Log(F32 value) { return std::log(value); }
	static inline F32 Exp(F32 value) { return std::exp(value); }
	static inline F32 Pow(F32 value, F32 exponent) { return std::pow(value, exponent); }

	template <typename T>
	static constexpr T Ceil(T value)
	{ 
		const T valueWhole = T(static_cast<I32>(value));
		return valueWhole + T((value > (T(0)) && (value > valueWhole)) ? 1 : 0);
	}
	template <typename T>
	static constexpr T Floor(T value)
	{
		const T valueWhole = T(static_cast<I32>(value));
		return valueWhole - T((value < (T(0)) && (value < valueWhole)) ? 1 : 0);
	}
	template <typename T>
	static constexpr T Round(T value) { return Floor(value + T(0.5)); }

	template <typename T>
	static constexpr T Min(T x, T y) { return (x <= y) ? x : y; }
	template <typename T>
	static constexpr T Max(T x, T y) { return (x <= y) ? y : x; }
	template <typename T>
	static constexpr T Clamp(T value, T valueMin, T valueMax) { return Max(Min(value, valueMax), valueMin); }
	template <typename T>
	static constexpr T Abs(T value) { return (value >= 0) ? value : -value; }

	template <typename T>
	static constexpr T Hermite(T value) { return value * value * (3 - 2 * value); }
	template <typename T>
	static constexpr T Quintic(T value) { return value * value * value * (value * (value * 6 - 15) + 10); }

	template <typename T>
	static constexpr T Lerp(T start, T end, T percent) { return start + percent * (end - start); }
	template <typename T>
	static constexpr T Bezier(T k, T c) { return c * 2 * k * (1 - k) + k * k; }

	template <typename T>
	static constexpr bool InRange(T value, T valueMin, T valueMax) { return value >= valueMin && value <= valueMax; }
	static constexpr bool Equals(F32 x, F32 y, F32 epsilon = Epsilon) { return Abs(x - y) < epsilon; }
	static constexpr bool Equals(I32 x, I32 y) { return x == y; }
	static constexpr bool Equals(U32 x, U32 y) { return x == y; }
};

} // namespace en