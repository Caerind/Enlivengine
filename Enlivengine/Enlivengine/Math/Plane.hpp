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

class Plane
{
public:
	Plane();
	Plane(const Vector3f& normal, F32 constant);
	Plane(const Vector3f& normal, const Vector3f& point);
	Plane(F32 a, F32 b, F32 c, F32 d);
	Plane(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3);

	void Set(const Vector3f& normal, F32 constant);
	void Set(const Vector3f& normal, const Vector3f& point);
	void Set(F32 a, F32 b, F32 c, F32 d);
	void Set(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3);

	const Vector3f& GetNormal() const;
	void SetNormal(const Vector3f& normal);

	F32 GetConstant() const;
	void SetConstant(F32 constant);

	bool operator==(const Plane& other) const;
	bool operator!=(const Plane& other) const;

	void Normalize();
	void Negate();

	enum class Side
	{
		Positive,
		Negative,
		Both
	};
	Side GetSide(const Vector3f& point) const;

	Vector3f GetAnyPoint() const;

	F32 GetDistance(const Vector3f& point) const;
	Vector3f GetClosestPoint(const Vector3f& point);

	bool Contains(const Vector3f& point) const;
	bool Contains(const Ray& ray) const;

	bool Intersects(const AABB& aabb, Side* side = nullptr) const;
	bool Intersects(const Frustum& frustum, Side* side = nullptr) const;
	bool Intersects(const Plane& plane) const;
	bool Intersects(const Ray& ray, Side* side = nullptr) const;
	bool Intersects(const Sphere& sphere, Side* side = nullptr) const;

	Plane Transform(const Matrix4f& transform) const;

private:
	Vector3f mNormal;
	F32 mConstant;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Plane, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("normal", &en::Plane::GetNormal, &en::Plane::SetNormal),
	ENLIVE_META_CLASS_MEMBER("constant", &en::Plane::GetConstant, &en::Plane::SetConstant)
ENLIVE_META_CLASS_END()
