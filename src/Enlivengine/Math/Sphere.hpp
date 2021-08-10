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

class Sphere
{
public:
	Sphere();
	Sphere(const Vector3f& center, F32 radius = 1.0f);
	Sphere(F32 radius, const Vector3f& center = Vector3f::Zero());

	const Vector3f& GetCenter() const;
	void SetCenter(const Vector3f& center);

	F32 GetRadius() const;
	void SetRadius(F32 radius);

	bool operator==(const Sphere& other) const;
	bool operator!=(const Sphere& other) const;

	F32 GetDistance(const Vector3f& point) const;
	F32 GetVolume() const;

	bool Contains(const Vector3f& point) const;
	bool Contains(const AABB& aabb) const;
	bool Contains(const Frustum& frustum) const;
	bool Contains(const Sphere& sphere) const;

	bool Intersects(const AABB& aabb) const;
	bool Intersects(const Frustum& frustum) const;
	bool Intersects(const Plane& plane) const;
	bool Intersects(const Ray& ray) const;
	bool Intersects(const Sphere& sphere) const;

	void Merge(const Vector3f& point);

	Sphere Transform(const Matrix4f& transform) const;

private:
	Vector3f mCenter;
	F32 mRadius;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Sphere, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("center", &en::Sphere::GetCenter, &en::Sphere::SetCenter),
	ENLIVE_META_CLASS_MEMBER("radius", &en::Sphere::GetRadius, &en::Sphere::SetRadius)
ENLIVE_META_CLASS_END()
