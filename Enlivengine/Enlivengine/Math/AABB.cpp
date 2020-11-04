#include <Enlivengine/Math/AABB.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>
#include <Enlivengine/Math/Sphere.hpp>

namespace en 
{

AABB::AABB()
	: mMin()
	, mMax() 
{
}

AABB::AABB(const Vector3f& min, const Vector3f& max) 
	: mMin(min)
	, mMax(max) 
{
}

AABB::AABB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ) 
	: mMin(minX, minY, minZ)
	, mMax(maxX, maxY, maxZ) 
{
}

void AABB::Set(const Vector3f& min, const Vector3f& max)
{
	mMin = min; 
	mMax = max;
}

void AABB::Set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ)
{
	mMin.Set(minX, minY, minZ);
	mMax.Set(maxX, maxY, maxZ);
}

void AABB::SetCenteredBox(const Vector3f& center, const Vector3f& halfSize)
{
	mMin = center - halfSize;
	mMax = center + halfSize;
}

void AABB::SetCenteredBox(F32 cX, F32 cY, F32 cZ, F32 hsX, F32 hsY, F32 hsZ)
{
	mMin.Set(cX - hsX, cY - hsY, cZ - hsZ);
	mMax.Set(cX + hsX, cY + hsY, cZ + hsZ);
}

const Vector3f& AABB::GetMin() const 
{ 
	return mMin;
}

void AABB::SetMin(const Vector3f& min) 
{ 
	mMin = min;
}

const Vector3f& AABB::GetMax() const 
{
	return mMax;
}

void AABB::SetMax(const Vector3f& max) 
{ 
	mMax = max;
}

bool AABB::operator==(const AABB& box) const
{
	return mMin == box.mMin && mMax == box.mMax;
}

bool AABB::operator!=(const AABB& box) const
{
	return !operator==(box);
}

Vector3f AABB::GetCenter() const
{ 
	return (mMin + mMax) * 0.5f;
}

Vector3f AABB::GetSize() const 
{ 
	return mMax - mMin; 
}

Vector3f AABB::GetHalfSize() const 
{ 
	return (mMax - mMin) * 0.5f; 
}

F32 AABB::GetVolume() const
{ 
	const Vector3f size = GetSize();
	return size.x * size.y * size.y;
}

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
Vector3f AABB::GetCorner(U32 index) const
{
	switch (index)
	{
	case 0:
		return mMin;
	case 1:
		return Vector3f(mMin.x, mMax.y, mMin.z);
	case 2:
		return Vector3f(mMax.x, mMax.y, mMin.z);
	case 3:
		return Vector3f(mMax.x, mMin.y, mMin.z);
	case 4:
		return Vector3f(mMax.x, mMin.y, mMax.z);
	case 5:
		return Vector3f(mMin.x, mMin.y, mMax.z);
	case 6:
		return Vector3f(mMin.x, mMax.y, mMax.z);
	case 7:
		return mMax;
	default:
		break;
	}
	return Vector3f();
}

F32 AABB::GetDistanceSquared(const Vector3f& point) const
{
	if (Contains(point))
	{
		return 0.0f;
	}
	else
	{
		Vector3f maxDist(0.0f);
		F32 x = point.x;
		F32 y = point.y;
		F32 z = point.z;

		if (x < mMin.x)
			maxDist.x = mMin.x - x;
		else if (x > mMax.x)
			maxDist.x = x - mMax.x;

		if (y < mMin.y)
			maxDist.y = mMin.y - y;
		else if (y > mMax.y)
			maxDist.y = y - mMax.y;

		if (z < mMin.z)
			maxDist.z = mMin.z - z;
		else if (z > mMax.z)
			maxDist.z = z - mMax.z;

		return maxDist.GetSquaredLength();
	}
}

F32 AABB::GetDistance(const Vector3f& point) const
{
	return Math::FastSqrt(GetDistanceSquared(point));
}

bool AABB::Contains(const Vector3f& point) const
{
	return point >= mMin && point <= mMax;
}

bool AABB::Contains(const AABB& box) const
{
	return box.mMin >= mMin && box.mMax <= mMax;
}

bool AABB::Contains(const Frustum& frustum) const
{
	for (U32 i = 0; i < 8; ++i)
	{
		if (!Contains(frustum.GetCorner(i)))
		{
			return false;
		}
	}
	return true;
}

bool AABB::Contains(const Sphere& sphere) const
{
	AABB sphereAABB;
	sphereAABB.SetCenteredBox(sphere.GetCenter(), Vector3f(sphere.GetRadius()));
	return Contains(sphereAABB);
}

bool AABB::Intersects(const AABB& box, AABB* intersection /*= nullptr*/) const
{
	const F32 left = Math::Max(mMin.x, box.mMin.x);
	const F32 right = Math::Min(mMax.x, box.mMax.x);
	if (left >= right)
		return false;

	const F32 top = Math::Max(mMin.y, box.mMin.y);
	const F32 bottom = Math::Min(mMax.y, box.mMax.y);
	if (top >= bottom)
		return false;

	const F32 down = Math::Max(mMin.z, box.mMin.z);
	const F32 up = Math::Min(mMax.z, box.mMax.z);
	if (down >= up)
		return false;

	if (intersection)
	{
		intersection->Set(left, top, down, right, bottom, up);
	}

	return true;
}

bool AABB::Intersects(const Frustum& frustum) const
{
	return frustum.Intersects(*this);
}

bool AABB::Intersects(const Plane& plane) const
{
	return plane.Intersects(*this);
}

bool AABB::Intersects(const Ray& ray) const
{
	return ray.Intersects(*this);
}

bool AABB::Intersects(const Sphere& sphere) const
{
	return sphere.Intersects(*this);
}

void AABB::Merge(const Vector3f& point)
{
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);
	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

void AABB::Merge(const AABB& aabb)
{
	mMin.x = Math::Min(mMin.x, aabb.mMin.x);
	mMin.y = Math::Min(mMin.y, aabb.mMin.y);
	mMin.z = Math::Min(mMin.z, aabb.mMin.z);
	mMax.x = Math::Max(mMax.x, aabb.mMax.x);
	mMax.y = Math::Max(mMax.y, aabb.mMax.y);
	mMax.z = Math::Max(mMax.z, aabb.mMax.z);
}

AABB AABB::Transform(const Matrix4f& transform) const
{
	const Vector3f p1 = transform.TransformPoint(mMin);
	const Vector3f p2 = transform.TransformPoint(mMax);
	AABB out;
	if (p1.x < p2.x)
	{
		out.mMin.x = p1.x;
		out.mMax.x = p2.x;
	}
	else
	{
		out.mMin.x = p2.x;
		out.mMax.x = p1.x;
	}
	if (p1.y < p2.y)
	{
		out.mMin.y = p1.y;
		out.mMax.y = p2.y;
	}
	else
	{
		out.mMin.y = p2.y;
		out.mMax.y = p1.y;
	}
	if (p1.z < p2.z)
	{
		out.mMin.z = p1.z;
		out.mMax.z = p2.z;
	}
	else
	{
		out.mMin.z = p2.z;
		out.mMax.z = p1.z;
	}
	return out;
}

} // namespace en

#endif // ENLIVE_MODULE_MATH
