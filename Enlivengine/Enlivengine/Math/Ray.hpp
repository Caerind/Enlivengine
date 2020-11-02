#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector3.hpp>

namespace en
{

class Ray
{
public:
	constexpr Ray() : mOrigin(), mDirection(Vector3f::UnitX()) {}
	constexpr Ray(const Vector3f& origin, const Vector3f& direction) : mOrigin(origin), mDirection(direction) {}

	constexpr const Vector3f& GetOrigin() const { return mOrigin; }
	constexpr void SetOrigin(const Vector3f& origin) { mOrigin = origin; }

	constexpr const Vector3f& GetDirection() const { return mDirection; }
	constexpr void SetDirection(const Vector3f& direction) { mDirection = direction; }

	constexpr Vector3f GetPoint(F32 t) const { return mOrigin + mDirection * t; }

	constexpr bool operator==(const Ray& other) const { return mOrigin == other.mOrigin && mDirection == other.mDirection; }
	constexpr bool operator!=(const Ray& other) const { return !operator==(other); }

private:
	Vector3f mOrigin;
	Vector3f mDirection;
};

} // namespace en

#endif // ENLIVE_MODULE_MATH