#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector3.hpp>

namespace en
{

class Plane
{
public:
	constexpr Plane() : mNormal(), mConstant() {}
	constexpr Plane(const Vector3f& normal, F32 constant) : mNormal(normal), mConstant(constant) {}
	constexpr Plane(const Vector3f& normal, const Vector3f& point) : mNormal(normal), mConstant(-normal.DotProduct(point)) {}
	constexpr Plane(F32 a, F32 b, F32 c, F32 d) : mNormal(a, b, c), mConstant(d) {}
	inline Plane(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3) : mNormal(), mConstant() { Set(point1, point2, point3); }

	constexpr void Set(const Vector3f& normal, F32 constant) { mNormal = normal; mConstant = constant; }
	constexpr void Set(const Vector3f& normal, const Vector3f& point) { mNormal = normal; mConstant = -normal.DotProduct(point); }
	constexpr void Set(F32 a, F32 b, F32 c, F32 d) { mNormal.Set(a, b, c); mConstant = d; }
	inline void Set(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3)
	{
		const Vector3f edge1 = point2 - point1;
		const Vector3f edge2 = point3 - point1;
		mNormal = edge1.CrossProduct(edge2).Normalized();
		mConstant = -mNormal.DotProduct(point1);
	}

	constexpr const Vector3f& GetNormal() const { return mNormal; }
	constexpr void SetNormal(const Vector3f& normal) { mNormal = normal; }

	constexpr F32 GetConstant() const { return mConstant; }
	constexpr void SetConstant(F32 constant) { mConstant = constant; }

	enum class Side
	{
		Positive,
		Negative,
		Both
	};
	constexpr Side GetSide(const Vector3f& point) const
	{
		const F32 distance = GetDistance(point);
		if (distance > 0.0f)
		{
			return Side::Positive;
		}
		else if (distance < 0.0f)
		{
			return Side::Negative;
		}
		else
		{
			return Side::Both;
		}
	}

	constexpr F32 GetDistance(const Vector3f& point) const { return mNormal.DotProduct(point) + mConstant; }
	constexpr Vector3f GetClosestPoint(const Vector3f& point) { return point - mNormal * GetDistance(point); }

	constexpr bool operator==(const Plane& other) const { return mConstant == other.mConstant && mNormal == other.mNormal; }
	constexpr bool operator!=(const Plane& other) const { return !operator==(other); }

private:
	Vector3f mNormal;
	F32 mConstant;
};

} // namespace en

#endif // ENLIVE_MODULE_MATH
