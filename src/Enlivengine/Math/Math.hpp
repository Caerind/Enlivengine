#pragma once

#include <cmath>

#define GLM_FORCE_XYZW_ONLY // Otherwise, there are unions that makes debugger less clear
#define GLM_FORCE_SILENT_WARNINGS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Utils/Serializer.hpp>
#include <Enlivengine/Utils/ObjectEditor.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

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
	static F32 Sqrt(F32 value) { return std::sqrt(value); }
	static F32 InvSqrt(F32 value) { return 1.0f / std::sqrt(value); }
	static F32 FastSqrt(F32 value) { return glm::fastSqrt<F32>(value); }
	static F32 FastInvSqrt(F32 value) { return glm::fastInverseSqrt<F32>(value); }

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

	static F32 Sin(F32 value) { return std::sin(value * kDegToRad); }
	static F32 Cos(F32 value) { return std::cos(value * kDegToRad); }
	static F32 Tan(F32 value) { return std::tan(value * kDegToRad); }
	static F32 Asin(F32 value) { return std::asin(value) * kRadToDeg; }
	static F32 Acos(F32 value) { return std::acos(value) * kRadToDeg; }
	static F32 Atan(F32 value) { return std::atan(value) * kRadToDeg; }
	static F32 Atan2(F32 x, F32 y) { return std::atan2(y, x) * kRadToDeg; }

	static F32 FastSin(F32 value) { return glm::fastSin(value * kDegToRad); }
	static F32 FastCos(F32 value) { return glm::fastCos(value * kDegToRad); }
	static F32 FastTan(F32 value) { return glm::fastTan(value * kDegToRad); }
	static F32 FastAsin(F32 value) { return glm::fastAsin(value) * kRadToDeg; }
	static F32 FastAcos(F32 value) { return glm::fastAcos(value) * kRadToDeg; }
	static F32 FastAtan(F32 value) { return glm::fastAtan(value) * kRadToDeg; }
	static F32 FastAtan2(F32 x, F32 y) { return glm::fastAtan(y, x) * kRadToDeg; }

	template <typename T>
	static T Ceil(T value) { return std::ceil(value); }
	template <typename T>
	static T Floor(T value) { return std::floor(value); }
	template <typename T>
	static T Round(T value) { return std::round(value); }

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