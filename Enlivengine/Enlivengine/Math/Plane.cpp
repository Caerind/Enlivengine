#include <Enlivengine/Math/Plane.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Sphere.hpp>
#include <Enlivengine/Math/Ray.hpp>

namespace en
{

Plane::Plane()
{
}

Plane::Plane(const Vector3f& normal, F32 constant)
	: mNormal(normal)
	, mConstant(constant)
{
}

Plane::Plane(const Vector3f& normal, const Vector3f& point)
{
	set(normal, point);
}

Plane::Plane(F32 a, F32 b, F32 c, F32 d)
{
	set(a, b, c, d);
}

Plane::Plane(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3)
{
	set(point1, point2, point3);
}

void Plane::set(const Vector3f& normal, F32 constant)
{
	setNormal(normal);
	mConstant = constant;
}

void Plane::set(const Vector3f& normal, const Vector3f& point)
{
	setNormal(normal);
	mConstant = -mNormal.DotProduct(point);
}

void Plane::set(F32 a, F32 b, F32 c, F32 d)
{
	mNormal.Set(a, b, c);
	mConstant = d;
	normalize();
}

void Plane::set(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3)
{
	Vector3f edge1 = point2 - point1;
	Vector3f edge2 = point3 - point1;
	mNormal.Set(edge1.CrossProduct(edge2).Normalized());
	mConstant = -mNormal.DotProduct(point1);
}

const Vector3f& Plane::getNormal() const
{
	return mNormal;
}

void Plane::setNormal(const Vector3f& normal)
{
	if (Math::Equals(normal.GetSquaredLength(), 1.0f))
	{
		mNormal = normal;
	}
	else
	{
		mNormal = normal.Normalized();
	}
}

F32 Plane::getConstant() const
{
	return mConstant;
}

void Plane::setConstant(F32 constant)
{
	mConstant = constant;
}

F32 Plane::normalize()
{
	F32 length = mNormal.GetLength();
	enAssert(length > 0.0f);
	F32 inv = 1.0f / length;
	mNormal *= inv;
	mConstant *= inv;
	return length;
}

Plane Plane::normalized() const
{
	Plane p(*this);
	p.normalize();
	return p;
}

Plane::Side Plane::getSide(const Vector3f& point) const
{
	F32 distance = getDistance(point);
	if (distance > 0.0f)
	{
		return Plane::Positive;
	}
	if (distance < 0.0f)
	{
		return Plane::Negative;
	}
	return Plane::None;
}

Plane::Side Plane::getSide(const AABB& box) const
{
	F32 distance = getDistance(box.getCenter());
	F32 maxAbsDistance = Math::Abs(mNormal.DotProduct(box.getHalfSize()));
	if (distance < -maxAbsDistance)
	{
		return Plane::Side::Negative;
	}
	if (distance > +maxAbsDistance)
	{
		return Plane::Side::Positive;
	}
	return Plane::Side::Both;
}

Plane::Side Plane::getSide(const Sphere& sphere) const
{
	F32 distance = getDistance(sphere.getCenter());
	if (sphere.getRadius() > Math::Abs(distance))
	{
		return Plane::Side::Both;
	}
	if (distance > 0.0f)
	{
		return Plane::Side::Positive;
	}
	if (distance < 0.0f)
	{
		return Plane::Side::Negative;
	}
	return Plane::Side::None;
}

Plane::Side Plane::getSide(const Ray& ray) const
{
	if (intersects(ray))
	{
		return Plane::Side::Both;
	}
	else
	{
		return getSide(ray.getOrigin());
	}
}

Vector3f Plane::getClosestPoint(const Vector3f& point)
{
	return point - mNormal * getDistance(point);
}

bool Plane::operator==(const Plane& p) const
{
	return Vector4f(mNormal, mConstant).Normalized() == Vector4f(p.mNormal, p.mConstant).Normalized();
}

bool Plane::operator!=(const Plane& p) const
{
	return !operator==(p);
}

F32 Plane::getDistance(const Vector3f& point) const
{
	return mNormal.DotProduct(point) + mConstant;
}

bool Plane::contains(const Vector3f& point) const
{
	return Math::Equals(getDistance(point), 0.0f);
}

bool Plane::contains(const Ray& ray) const
{
	return Math::Equals(mNormal.DotProduct(ray.getDirection()), 0.0f) && contains(ray.getOrigin());
}

bool Plane::intersects(const AABB& box) const
{
	return getSide(box) == Plane::Side::Both;
}

bool Plane::intersects(const Sphere& sphere) const
{
	return Math::Abs(getDistance(sphere.getCenter())) <= sphere.getRadius();
}

bool Plane::intersects(const Plane& plane) const
{
	Plane p1 = normalized();
	Plane p2 = plane.normalized();
	if (p1.mNormal == p2.mNormal)
	{
		return Math::Equals(p1.mConstant, p2.mConstant);
	}
	else
	{
		return true;
	}
}

bool Plane::intersects(const Ray& ray) const
{
	return ray.intersects(*this);
}

} // namespace en
