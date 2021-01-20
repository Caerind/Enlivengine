#pragma once

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>

namespace en
{

class AABB;
class Frustum;
class Plane;
class Ray;
class Sphere;

class Ray
{
public:
	Ray();
	Ray(const Vector3f& origin, const Vector3f& direction);

	const Vector3f& GetOrigin() const;
	void SetOrigin(const Vector3f& origin);

	const Vector3f& GetDirection() const;
	void SetDirection(const Vector3f& direction);

	bool operator==(const Ray& other) const;
	bool operator!=(const Ray& other) const;

	Vector3f GetPoint(F32 t) const;

	bool Contains(const Vector3f& point) const;

	bool Intersects(const AABB& aabb, F32* distance = nullptr) const;
	bool Intersects(const Frustum& frustum, F32* distance = nullptr) const;
	bool Intersects(const Plane& plane, F32* distance = nullptr) const;
	bool Intersects(const Ray& ray, F32* distance = nullptr, F32* distanceOther = nullptr) const;
	bool Intersects(const Sphere& sphere, F32* distance = nullptr) const;

	Ray Transform(const Matrix4f& transform) const;

private:
	Vector3f mOrigin;
	Vector3f mDirection;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Ray, true, true)
	ENLIVE_META_CLASS_MEMBER("origin", &en::Ray::GetOrigin, &en::Ray::SetOrigin),
	ENLIVE_META_CLASS_MEMBER("direction", &en::Ray::GetDirection, &en::Ray::SetDirection)
ENLIVE_META_CLASS_END()