#include <Enlivengine/Math/Sphere.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>

namespace en 
{

Sphere::Sphere()
	: mCenter()
	, mRadius(1.0f)
{
}

Sphere::Sphere(const Vector3f& center, F32 radius /*= 1.0f*/)
	: mCenter(center)
	, mRadius(radius)
{
}

Sphere::Sphere(F32 radius, const Vector3f& center /*= Vector3f::Zero()*/)
	: mCenter(center)
	, mRadius(radius)
{
}

const Vector3f& Sphere::GetCenter() const
{
	return mCenter;
}

void Sphere::SetCenter(const Vector3f& center)
{
	mCenter = center;
}

F32 Sphere::GetRadius() const
{
	return mRadius;
}

void Sphere::SetRadius(F32 radius)
{
	mRadius = radius;
}

bool Sphere::operator==(const Sphere& other) const
{
	return mCenter == other.mCenter && mRadius == other.mRadius;
}

bool Sphere::operator!=(const Sphere& other) const
{
	return !operator==(other);
}

F32 Sphere::GetDistance(const Vector3f& point) const
{
	return (mCenter - point).GetLength() - mRadius;
}

F32 Sphere::GetVolume() const
{
	static constexpr F32 pi4over3 = (4.0f / 3.0f) * Math::Pi;
	return pi4over3 * mRadius;
}

bool Sphere::Contains(const Vector3f& point) const
{
	return (mCenter - point).GetSquaredLength() <= mRadius * mRadius;
}

bool Sphere::Contains(const AABB& aabb) const
{
	return Contains(aabb.GetMin()) && Contains(aabb.GetMax());
}

bool Sphere::Contains(const Frustum& frustum) const
{
	return frustum.Contains(*this);
}

bool Sphere::Contains(const Sphere& sphere) const
{
	if (mRadius < sphere.mRadius || !sphere.Contains(mCenter))
	{
		return false;
	}
	const Vector3f d = mCenter - sphere.mCenter;
	return (sphere.mRadius - d.GetLength() - mRadius > 0.0f);
}

bool Sphere::Intersects(const AABB& aabb) const
{
	// Use splitting planes
	const Vector3f& min(aabb.GetMin());
	const Vector3f& max(aabb.GetMax());
	// Arvo's algorithm
	F32 s;
	F32 d = 0.f;
	for (U32 i = 0; i < 3; ++i)
	{
		if (mCenter[i] < min[i])
		{
			s = mCenter[i] - min[i];
			d += s * s;
		}
		else if (mCenter[i] > max[i])
		{
			s = mCenter[i] - max[i];
			d += s * s;
		}
	}
	return d <= mRadius * mRadius;
}

bool Sphere::Intersects(const Frustum& frustum) const
{
	return frustum.Intersects(*this);
}

bool Sphere::Intersects(const Plane& plane) const
{
	return plane.Intersects(*this);
}

bool Sphere::Intersects(const Ray& ray) const
{
	return ray.Intersects(*this);
}

bool Sphere::Intersects(const Sphere& sphere) const
{
	const F32 rr = (mRadius + sphere.mRadius);
	return (mCenter - sphere.mCenter).GetSquaredLength() <= rr * rr;
}

void Sphere::Merge(const Vector3f& point)
{
	const F32 d = GetDistance(point);
	if (d > 0.0f)
	{
		mRadius += d;
	}
}

Sphere Sphere::Transform(const Matrix4f& transform) const
{
	const Vector3f scale = transform.GetScale();
	const F32 maxScale = Math::Max(scale.x, Math::Max(scale.y, scale.z));
	return Sphere(transform.TransformPoint(mCenter), mRadius * maxScale);
}

} // namespace en
