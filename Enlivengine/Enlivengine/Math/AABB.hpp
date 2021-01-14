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

class AABB
{
public:
	AABB();
	AABB(const Vector3f& min, const Vector3f& max);
	AABB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ);

	void Set(const Vector3f& min, const Vector3f& max);
	void Set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ);
	void SetCenteredBox(const Vector3f& center, const Vector3f& halfSize);
	void SetCenteredBox(F32 cX, F32 cY, F32 cZ, F32 hsX, F32 hsY, F32 hsZ);

	const Vector3f& GetMin() const;
	void SetMin(const Vector3f& min);

	const Vector3f& GetMax() const;
	void SetMax(const Vector3f& max);

	bool operator==(const AABB& box) const;
	bool operator!=(const AABB& box) const;

	Vector3f GetCenter() const;
	Vector3f GetSize() const;
	Vector3f GetHalfSize() const;
	F32 GetVolume() const;

	/*
	   1-------2
	  /|      /|
	 / |     / |
	6-------7  |
	|  0----|--3
	| /     | /
	|/      |/
	5-------4
	*/
	Vector3f GetCorner(U32 index) const;

	F32 GetDistanceSquared(const Vector3f& point) const;
	F32 GetDistance(const Vector3f& point) const;

	bool Contains(const Vector3f& point) const;
	bool Contains(const AABB& aabb) const;
	bool Contains(const Frustum& frustum) const;
	bool Contains(const Sphere& sphere) const;

	bool Intersects(const AABB& box, AABB* intersection = nullptr) const;
	bool Intersects(const Frustum& frustum) const;
	bool Intersects(const Plane& plane) const;
	bool Intersects(const Ray& ray) const;
	bool Intersects(const Sphere& sphere) const;

	void Merge(const Vector3f& point);
	void Merge(const AABB& aabb);

	AABB Transform(const Matrix4f& transform) const;

private:
	Vector3f mMin;
	Vector3f mMax;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::AABB)
	ENLIVE_META_CLASS_MEMBER("min", &en::AABB::GetMin, &en::AABB::SetMin),
	ENLIVE_META_CLASS_MEMBER("max", &en::AABB::GetMax, &en::AABB::SetMax)
ENLIVE_META_CLASS_END()