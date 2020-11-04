#include <Enlivengine/Math/Plane.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Ray.hpp>
#include <Enlivengine/Math/Sphere.hpp>

namespace en 
{

Plane::Plane()
	: mNormal(Vector3f::UnitY())
	, mConstant(0.0f)
{
}

Plane::Plane(const Vector3f& normal, F32 constant)
	: mNormal()
	, mConstant()
{
	Set(normal, constant);
}

Plane::Plane(const Vector3f& normal, const Vector3f& point) 
	: mNormal()
	, mConstant() 
{
	Set(normal, point);
}

Plane::Plane(F32 a, F32 b, F32 c, F32 d) 
	: mNormal()
	, mConstant()
{
	Set(a, b, c, d);
}

Plane::Plane(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3) 
	: mNormal()
	, mConstant() 
{ 
	Set(point1, point2, point3); 
}

void Plane::Set(const Vector3f& normal, F32 constant)
{
	mNormal = normal;
	mConstant = constant;
}

void Plane::Set(const Vector3f& normal, const Vector3f& point)
{
	mNormal = normal;
	mConstant = -normal.DotProduct(point);
}

void Plane::Set(F32 a, F32 b, F32 c, F32 d)
{ 
	mNormal.Set(a, b, c); 
	mConstant = d;
}

void Plane::Set(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3)
{
	const Vector3f edge1 = point2 - point1;
	const Vector3f edge2 = point3 - point1;
	mNormal = edge1.CrossProduct(edge2).Normalized();
	mConstant = -mNormal.DotProduct(point1);
}

const Vector3f& Plane::GetNormal() const
{ 
	return mNormal; 
}

void Plane::SetNormal(const Vector3f& normal)
{
	mNormal = normal;
}

F32 Plane::GetConstant() const
{
	return mConstant; 
}

void Plane::SetConstant(F32 constant)
{ 
	mConstant = constant; 
}

void Plane::Normalize()
{
	const F32 inv = 1.0f / mNormal.GetLength();
	mNormal *= inv;
	mConstant *= inv;
}

void Plane::Negate()
{
	mNormal *= -1.0f;
	mConstant *= -1.0f;
}

bool Plane::operator==(const Plane& other) const
{
	return mConstant == other.mConstant && mNormal == other.mNormal;
}

bool Plane::operator!=(const Plane& other) const
{
	return !operator==(other);
}

Plane::Side Plane::GetSide(const Vector3f& point) const
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

Vector3f Plane::GetAnyPoint() const
{
	return -mNormal * mConstant;
}

F32 Plane::GetDistance(const Vector3f& point) const
{ 
	return mNormal.DotProduct(point) + mConstant;
}

Vector3f Plane::GetClosestPoint(const Vector3f& point)
{ 
	return point - mNormal * GetDistance(point);
}

bool Plane::Contains(const Vector3f& point) const
{
	return Math::Equals(GetDistance(point), 0.0f);
}

bool Plane::Contains(const Ray& ray) const
{
	return Math::Equals(mNormal.DotProduct(ray.GetDirection()), 0.0f) && Contains(ray.GetOrigin());
}

bool Plane::Intersects(const AABB& aabb, Side* side /*= nullptr*/) const
{
	const F32 distance = GetDistance(aabb.GetCenter());
	const F32 maxAbsDistance = Math::Abs(mNormal.DotProduct(aabb.GetHalfSize()));
	if (distance < -maxAbsDistance)
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Negative;
		}
		return false;
	}
	else if (distance > +maxAbsDistance)
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Positive;
		}
		return false;
	}
	else
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Both;
		}
		return true;
	}
}

bool Plane::Intersects(const Frustum& frustum, Side* side /*= nullptr*/) const
{
	Side firstSide = GetSide(frustum.GetCorner(0));
	for (U32 i = 1; i < 8; ++i)
	{
		if (GetSide(frustum.GetCorner(i)) != firstSide)
		{
			if (side != nullptr)
			{
				*side = Plane::Side::Both;
			}
			return true;
		}
	}
	if (side != nullptr)
	{
		*side = firstSide;
	}
	return false;
}

bool Plane::Intersects(const Plane& plane) const
{
	return mNormal != plane.mNormal;
}

bool Plane::Intersects(const Ray& ray, Side* side /*= nullptr*/) const
{
	if (ray.Intersects(*this))
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Both;
		}
		return true;
	}
	else
	{
		if (side != nullptr)
		{
			*side = GetSide(ray.GetOrigin());
		}
		return false;
	}
}

bool Plane::Intersects(const Sphere& sphere, Side* side /*= nullptr*/) const
{
	const F32 distance = GetDistance(sphere.GetCenter());
	if (sphere.GetRadius() > Math::Abs(distance))
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Both;
		}
		return true;
	}
	if (distance > 0.0f)
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Positive;
		}
		return false;
	}
	else
	{
		if (side != nullptr)
		{
			*side = Plane::Side::Negative;
		}
		return false;
	}
}

Plane Plane::Transform(const Matrix4f& transform) const
{
	enAssert(false); // Not implemented yet
	ENLIVE_UNUSED(transform);
	return Plane();
}

} // namespace en

#endif // ENLIVE_MODULE_MATH
