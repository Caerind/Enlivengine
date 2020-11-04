#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

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

	const Vector3f& GetCenter() const;
	void SetCenter(const Vector3f& center);

	F32 GetRadius() const;
	void SetRadius(F32 radius);

	bool operator==(const Sphere& other) const;
	bool operator!=(const Sphere& other) const;

	F32 GetDistance(const Vector3f& point) const;

	bool Contains(const Vector3f& point) const;

	bool Contains(const AABB& aabb) const;
	bool Contains(const Frustum& frustum) const;
	bool Contains(const Sphere& sphere) const;

	bool Intersects(const AABB& aabb) const;
	bool Intersects(const Frustum& frustum) const;
	bool Intersects(const Plane& plane) const;
	bool Intersects(const Ray& ray) const;
	bool Intersects(const Sphere& sphere) const;

	Sphere Transform(const Matrix4f& transform) const;

private:
	Vector3f mCenter;
	F32 mRadius;
};

} // namespace en

#endif // ENLIVE_MODULE_MATH
