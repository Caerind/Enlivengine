#include <Enlivengine/Math/Sphere.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>

namespace en
{

Sphere::Sphere()
{
}

Sphere::Sphere(const Vector3f& center, F32 radius)
	: mCenter(center)
	, mRadius(radius)
{
}

Sphere::Sphere(F32 radius, const Vector3f& center)
	: mCenter(center)
	, mRadius(radius)
{
}

const Vector3f& Sphere::getCenter() const
{
	return mCenter;
}

void Sphere::setCenter(const Vector3f& center)
{
	mCenter.Set(center);
}

F32 Sphere::getRadius() const
{
	return mRadius;
}

void Sphere::setRadius(F32 radius)
{
	mRadius = radius;
}

bool Sphere::operator==(const Sphere& p) const
{
	return mCenter == p.mCenter && Math::Equals(mRadius, p.mRadius);
}

bool Sphere::operator!=(const Sphere& p) const
{
	return !operator==(p);
}

F32 Sphere::getDistance(const Vector3f& point) const
{
	return (mCenter - point).GetLength() - mRadius;
}

bool Sphere::contains(const Vector3f& point) const
{
	return (point - mCenter).GetSquaredLength() <= mRadius * mRadius;
}

bool Sphere::contains(const AABB& box) const
{
	Vector3f d = box.getCenter() - mCenter;
	if (d.GetLength() > mRadius)
	{
		return false;
	}

	if (d.x > 0.0f)
	{
		if (d.y > 0.0f)
		{
			if (d.z > 0.0f)
			{
				return contains(box.getCorner(7));
			}
			else
			{
				return contains(box.getCorner(2));
			}
		}
		else
		{
			if (d.z > 0.0f)
			{
				return contains(box.getCorner(4));
			}
			else
			{
				return contains(box.getCorner(3));
			}
		}
	}
	else
	{
		if (d.y > 0.0f)
		{
			if (d.z > 0.0f)
			{
				return contains(box.getCorner(6));
			}
			else
			{
				return contains(box.getCorner(1));
			}
		}
		else
		{
			if (d.z > 0.0f)
			{
				return contains(box.getCorner(5));
			}
			else
			{
				return contains(box.getCorner(0));
			}
		}
	}
}

bool Sphere::contains(const Sphere& sphere) const
{
	if (mRadius > sphere.mRadius || !sphere.contains(mCenter))
	{
		return false;
	}
	Vector3f d = mCenter - sphere.mCenter;
	return (sphere.mRadius - d.GetLength() - mRadius > 0.0f);
}

bool Sphere::intersects(const AABB& box) const
{
	// Use splitting planes
	const Vector3f& min(box.getMinimum());
	const Vector3f& max(box.getMaximum());
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

bool Sphere::intersects(const Sphere& sphere) const
{
	return (mCenter - sphere.mCenter).GetSquaredLength() <= (mRadius + sphere.mRadius) * (mRadius + sphere.mRadius);
}

bool Sphere::intersects(const Plane& plane) const
{
	return plane.intersects(*this);
}

bool Sphere::intersects(const Ray& ray) const
{
	return ray.intersects(*this);
}

AABB Sphere::getAABB() const
{
	return AABB(mCenter - Vector3f(mRadius), mCenter + Vector3f(mRadius));
}

void Sphere::fromAABB(const AABB& aabb)
{
	mCenter = aabb.getCenter();
	Vector3f hs = aabb.getHalfSize();
	mRadius = Math::Max(hs.x, Math::Max(hs.y, hs.z));
}

void Sphere::merge(const Vector3f& point)
{
	F32 d = getDistance(point);
	if (d > 0.0f)
	{
		mRadius += d;
	}
}

void Sphere::transform(const Matrix4f& m)
{
	mCenter = m.transformPosition(mCenter);
}

} // namespace en
