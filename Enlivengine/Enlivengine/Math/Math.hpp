#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Utils/Serializer.hpp>

// TODO : Constexpr Sqrt => InvSqrt, Asin, Acos
// TODO : Real : Float/Double
// TODO : See https://github.com/pmttavara/pt_math
// TODO : See https://github.com/kthohr/gcem

namespace en
{

class Math
{
public:
	Math() = delete;

	enum class Handedness { Right, Left };

	static constexpr F32 Epsilon { 0.0001f };
	static constexpr F32 Pi { 3.141592653589793238462643383f };
	static constexpr F32 TwoPi { 2.0f * Pi };
	static constexpr F32 HalfPi { 0.5f * Pi };
	static constexpr F32 InvPi { 1.0f / Pi };
	static constexpr F32 InvTwoPi { 1.0f / TwoPi };
	static constexpr F32 kDegToRad { Pi / 180.0f };
	static constexpr F32 kRadToDeg{ 180.0f / Pi };

	template <typename T>
	static constexpr T Sqr(T value) { return value * value; }
	static inline F32 FastSqrt(F32 value)
	{
		I32 i = *(I32*)&value;
		i = (1 << 29) + (i >> 1) - (1 << 22);
		F32 f = *(F32*)&i;
		f = f + value / f;
		f = 0.25f * f + value / f;
		return f;
	}
	static inline F32 FastInvSqrt(F32 value)
	{
		// TODO : This may be improved
		return 1.0f / FastSqrt(value);
	}

	static constexpr F32 DegToRad(F32 value) { return value * kDegToRad; }
	static constexpr F32 RadToDeg(F32 value) { return value * kRadToDeg; }

	static constexpr F32 AngleMagnitude180(F32 value)
	{
		while (value >= 180.0f)
		{
			value -= 360.0f;
		};
		while (value < -180.0f)
		{
			value += 360.0f;
		};
		return value;
	}
	static constexpr F32 AngleMagnitude360(F32 value)
	{
		while (value >= 360.0f)
		{
			value -= 360.0f;
		};
		while (value < 0.0f)
		{
			value += 360.0f;
		};
		return value;
	}
	static constexpr F32 AngleBetween(F32 a, F32 b) { const F32 x = Abs(AngleMagnitude360(a) - AngleMagnitude360(b)); return (x < 180.0f) ? x : 360.0f - x; };
	static constexpr F32 AngleOpposite(F32 value) { return AngleMagnitude360(value - 180.0f); }

	static constexpr F32 Sin(F32 value)
	{
		F32 sign = 1.0f;
		value = AngleMagnitude360(value);
		if (value > 270.0f)
		{
			value = 360.0f - value;
			sign = -1.0f;
		}
		else if (value > 180.0f)
		{
			value -= 180.0f;
			sign = -1.0f;
		}
		else if (value > 90.0f)
		{
			value = 180.0f - value;
		}
		value *= kDegToRad;
		const F32 x2 = value * value;
		const F32 x4 = x2 * x2;
		const F32 x6 = x4 * x2;
		const F32 x8 = x6 * x2;
		const F32 x10 = x8 * x2;
		value *= (1.0f - 0.1666666664f * x2 + 0.0083333315f * x4 - 0.0001984090f * x6 + 0.0000027526f * x8 - 0.0000000239f * x10);
		return sign * value;
	}
	static constexpr F32 Cos(F32 value)
	{
		F32 sign = 1.0f;
		value = AngleMagnitude360(value);
		if (value > 270.0f)
		{
			value = 360.0f - value;
		}
		else if (value > 180.0f)
		{
			value -= 180.0f;
			sign = -1.0f;
		}
		else if (value > 90.0f)
		{
			value = 180.0f - value;
			sign = -1.0f;
		}
		value *= kDegToRad;
		const F32 x2 = value * value;
		const F32 x4 = x2 * x2;
		const F32 x6 = x4 * x2;
		const F32 x8 = x6 * x2;
		const F32 x10 = x8 * x2;
		value = (1.0f - 0.4999999963f * x2 + 0.0416666418f * x4 - 0.0013888397f * x6 + 0.0000247609f * x8 - 0.0000002605f * x10);
		return sign * value;
	}
	static constexpr F32 Tan(F32 value)
	{
		// TODO : This can be replaced with a Taylor development
		return Sin(value) / Cos(value);
	}

	static inline F32 Asin(F32 value)
	{
		const F32 sign = (value >= 0.0f) ? 1.0f : -1.0f;
		value = sign * value;
		if (value > 1.0f)
		{
			return sign * 90.0f;
		}
		const F32 x2 = value * value;
		const F32 x3 = x2 * value;
		const F32 x4 = x3 * value;
		const F32 x5 = x4 * value;
		const F32 x6 = x5 * value;
		const F32 x7 = x6 * value;
		const F32 sqrRoot = FastSqrt(1.0f - value);
		const F32 firstPart = sqrRoot * (1.5707963050f - 0.2145988016f * value + 0.0889789874f * x2 - 0.0501743046f * x3 + 0.0308918810f * x4 - 0.01708812556f * x5 + 0.0066700901f * x6 - 0.0012624911f * x7);
		return sign * (HalfPi - firstPart) * kRadToDeg;
	}
	static inline F32 Acos(F32 value)
	{
		return 90.0f - Asin(value);
	}
	static constexpr F32 Atan(F32 value)
	{
		F32 sign = (value >= 0.0f) ? 1.0f : -1.0f;
		value = sign * value;
		if (value <= 1.3333f)
		{
			if (value <= 1.0f)
			{
				const F32 x2 = value * value;
				const F32 x4 = x2 * x2;
				const F32 x6 = x4 * x2;
				const F32 x8 = x6 * x2;
				const F32 x10 = x8 * x2;
				const F32 x12 = x10 * x2;
				const F32 x14 = x12 * x2;
				const F32 x16 = x14 * x2;
				const F32 firstPart = 1.0f - 0.3333314528f * x2 + 0.1999355085f * x4 - 0.1420889944f * x6 + 0.1065626393f * x8 - 0.0752896400f * x10 + 0.0429096138f * x12 - 0.0161657367f * x14 + 0.0028662257f * x16;
				return kRadToDeg * sign * value * firstPart;
			}
			else
			{
				// Ai3
				const F32 p00 = 0.862170f;
				const F32 p01 = 0.423529f;
				const F32 p02 = -0.209273f;
				const F32 p03 = 0.078082f;
				const F32 p04 = -0.013556f;
				const F32 p05 = -0.009125f;
				const F32 p06 = 0.011342f;
				const F32 p07 = -0.006847f;
				const F32 p08 = 0.002318f;
				const F32 p09 = 0.000219f;
				const F32 p10 = -0.000981f;
				const F32 p11 = 0.000810f;
				const F32 p12 = -0.000374f;
				const F32 y = value - 1.166667f;
				return kRadToDeg * sign * (((((((((((((
					+p12) * y
					+ p11) * y
					+ p10) * y
					+ p09) * y
					+ p08) * y
					+ p07) * y
					+ p06) * y
					+ p05) * y
					+ p04) * y
					+ p03) * y
					+ p02) * y
					+ p01) * y
					+ p00);
			}
		}
		else // value > 1.333f
		{
			if (value > 2.0f)
			{
				// Tail
				const F32 p03 = -0.333333f;
				const F32 p05 = 0.200000f;
				const F32 p07 = -0.142857f;
				const F32 p09 = 0.111111f;
				const F32 p11 = -0.090908f;
				const F32 p13 = 0.076902f;
				const F32 p15 = -0.066468f;
				const F32 p17 = 0.057557f;
				const F32 p19 = -0.047098f;
				const F32 p21 = 0.031307f;
				const F32 p000 = 1.570796f;
				const F32 y = 1.0f / value;
				const F32 y2 = y * y;
				return kRadToDeg * sign * (-(((((((((((
					+p21) * y2
					+ p19) * y2
					+ p17) * y2
					+ p15) * y2
					+ p13) * y2
					+ p11) * y2
					+ p09) * y2
					+ p07) * y2
					+ p05) * y2
					+ p03) * y2 * y + y) + p000);
			}
			else
			{
				if (value <= 1.6666f)
				{
					// Ai4
					const F32 p00 = 0.982794f;
					const F32 p01 = 0.307692f;
					const F32 p02 = -0.142012f;
					const F32 p03 = 0.055834f;
					const F32 p04 = -0.016806f;
					const F32 p05 = 0.002103f;
					const F32 p06 = 0.001830f;
					const F32 p07 = -0.001910f;
					const F32 p08 = 0.001120f;
					const F32 p09 = -0.000462f;
					const F32 p10 = 0.000104f;
					const F32 p11 = 0.000031f;
					const F32 y = value - 1.500000f;
					return kRadToDeg * sign * ((((((((((((
						+p11) * y
						+ p10) * y
						+ p09) * y
						+ p08) * y
						+ p07) * y
						+ p06) * y
						+ p05) * y
						+ p04) * y
						+ p03) * y
						+ p02) * y
						+ p01) * y
						+ p00);
				}
				else
				{
					// Ai5
					const F32 p00 = 1.071450f;
					const F32 p01 = 0.229299f;
					const F32 p02 = -0.096393f;
					const F32 p03 = 0.036503f;
					const F32 p04 = -0.011967f;
					const F32 p05 = 0.003027f;
					const F32 p06 = -0.000291f;
					const F32 p07 = -0.000286f;
					const F32 p08 = 0.000260f;
					const F32 p09 = -0.000145f;
					const F32 p10 = 0.000061f;
					const F32 y = value - 1.833333f;
					return kRadToDeg * sign * (((((((((((
						+p10) * y
						+ p09) * y
						+ p08) * y
						+ p07) * y
						+ p06) * y
						+ p05) * y
						+ p04) * y
						+ p03) * y
						+ p02) * y
						+ p01) * y
						+ p00);
				}
			}
		}
	}
	static constexpr F32 Atan2(F32 x, F32 y)
	{
		const F32 ax = (x >= 0.0f) ? x : -x;
		const F32 signY = (y >= 0.0f) ? 1.0f : -1.0f;
		const F32 ay = signY * y;
		const bool invert = ay > ax;
		const F32 z = invert ? ax / ay : ay / ax;

		// Atan(z);
		const F32 x2 = z * z;
		const F32 x4 = x2 * x2;
		const F32 x6 = x4 * x2;
		const F32 x8 = x6 * x2;
		const F32 x10 = x8 * x2;
		const F32 x12 = x10 * x2;
		const F32 x14 = x12 * x2;
		const F32 x16 = x14 * x2;
		const F32 firstPart = 1.0f - 0.3333314528f * x2 + 0.1999355085f * x4 - 0.1420889944f * x6 + 0.1065626393f * x8 - 0.0752896400f * x10 + 0.0429096138f * x12 - 0.0161657367f * x14 + 0.0028662257f * x16;

		F32 th = kRadToDeg * z * firstPart;
		if (invert) th = 90.0f - th;
		if (x < 0.0f) th = 180.0f - th;
		return signY * th;
	}

	template <typename T>
	static constexpr T Ceil(T value)
	{
		const T valueWhole = T(static_cast<I32>(value));
		return valueWhole + T((value > (T(0)) && (value > valueWhole)) ? T(1) : T(0));
	}
	template <typename T>
	static constexpr T Floor(T value)
	{
		const T valueWhole = T(static_cast<I32>(value));
		return valueWhole - T((value < (T(0)) && (value < valueWhole)) ? T(1) : T(0));
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
	static constexpr T Abs(T value) { return (value >= T(0)) ? value : -value; }
	template <typename T>
	static constexpr T Sign(T value) { return (value >= T(0)) ? T(1) : T(-1); }

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