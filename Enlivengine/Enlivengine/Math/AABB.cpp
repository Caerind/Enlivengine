#include <Enlivengine/Math/AABB.hpp>

#include <Enlivengine/Math/Sphere.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>

namespace en
{

AABB::AABB()
{
}

AABB::AABB(const Vector3f& min, const Vector3f& max)
{
	setBox(min, max);
}

AABB::AABB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ)
{
	setBox(minX, minY, minZ, maxX, maxY, maxZ);
}

const Vector3f& AABB::getMinimum() const
{
	return mMin;
}

void AABB::setMinimum(const Vector3f& minimum)
{
	mMin.set(minimum);
}

void AABB::setMinimum(F32 x, F32 y, F32 z)
{
	mMin.set(x, y, z);
}

const Vector3f& AABB::getMaximum() const
{
	return mMax;
}

void AABB::setMaximum(const Vector3f& maximum)
{
	mMax.set(maximum);
}

void AABB::setMaximum(F32 x, F32 y, F32 z)
{
	mMax.set(x, y, z);
}

void AABB::setBox(const Vector3f& minimum, const Vector3f& maximum)
{
	mMin.set(minimum);
	mMax.set(maximum);
}

void AABB::setBox(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ)
{
	mMin.set(minX, minY, minZ);
	mMax.set(maxX, maxY, maxZ);
}

void AABB::setCenteredBox(const Vector3f& center, const Vector3f& halfSize)
{
	mMin.set(center - halfSize);
	mMax.set(center + halfSize);
}

void AABB::setCenteredBox(F32 cX, F32 cY, F32 cZ, F32 hsX, F32 hsY, F32 hsZ)
{
	setCenteredBox(Vector3f(cX, cY, cZ), Vector3f(hsX, hsY, hsZ));
}


Vector3f AABB::getCenter() const
{
	return (mMax + mMin) * 0.5f;
}

Vector3f AABB::getSize() const
{
	return mMax - mMin;
}

Vector3f AABB::getHalfSize() const
{
	return (mMax - mMin) * 0.5f;
}

F32 AABB::getVolume() const
{
	Vector3f d(getSize());
	return d.x * d.y * d.z;
}

Vector3f AABB::getCorner(U8 index) const
{
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
			return Vector3f();
	}
}

bool AABB::operator==(const AABB& box) const
{
	return mMin == box.mMin && mMax == box.mMax;
}

bool AABB::operator!=(const AABB& box) const
{
	return !operator==(box);
}

F32 AABB::getDistanceSquared(const Vector3f& point) const
{
	if (contains(point))
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

		return maxDist.getSquaredLength();
	}
}

F32 AABB::getDistance(const Vector3f& point) const
{
	return Math::Sqrt(getDistanceSquared(point));
}

bool AABB::contains(const Vector3f& point) const
{
	return point >= mMin && point <= mMax;
}

bool AABB::contains(const AABB& box) const
{
	return box.mMin >= mMin && box.mMax <= mMax;
}

bool AABB::contains(const Sphere& sphere) const
{
	return contains(sphere.getAABB());
}

bool AABB::intersects(const AABB& box, AABB* intersection) const
{
	F32 left = Math::Max(mMin.x, box.mMin.x);
	F32 right = Math::Min(mMax.x, box.mMax.x);
	if (left >= right)
		return false;

	F32 top = Math::Max(mMin.y, box.mMin.y);
	F32 bottom = Math::Min(mMax.y, box.mMax.y);
	if (top >= bottom)
		return false;

	F32 up = Math::Max(mMin.z, box.mMin.z);
	F32 down = Math::Min(mMax.z, box.mMax.z);
	if (up >= down)
		return false;

	if (intersection)
	{
		intersection->setBox(left, top, up, right, bottom, down);
	}

	return true;
}

bool AABB::intersects(const Sphere& sphere) const
{
	return sphere.intersects(*this);
}

bool AABB::intersects(const Plane& plane) const
{
	return plane.intersects(*this);
}

bool AABB::intersects(const Ray& ray) const
{
	return ray.intersects(*this);
}

const AABB& AABB::getAABB() const
{
	return *this;
}

void AABB::fromAABB(const AABB& aabb)
{
	mMin.set(aabb.mMin);
	mMax.set(aabb.mMax);
}

void AABB::merge(const Vector3f& point)
{
	mMin.minimize(point);
	mMax.maximize(point);
}

void AABB::merge(const AABB& box)
{
	mMin.minimize(box.mMin);
	mMax.maximize(box.mMax);
}

void AABB::transform(const Matrix4f& m)
{
	const Vector3f oldMin(mMin);
	const Vector3f oldMax(mMax);
	Vector3f currentCorner;
	mMin.set(0.0f);
	mMax.set(0.0f);
	// This order only changes one value to change the corner
	// min min min
	currentCorner = oldMin;
	merge(m * currentCorner);
	// min min max
	currentCorner.z = oldMax.z;
	merge(m * currentCorner);
	// min max max
	currentCorner.y = oldMax.y;
	merge(m * currentCorner);
	// min max min
	currentCorner.z = oldMin.z;
	merge(m * currentCorner);
	// max max min
	currentCorner.x = oldMax.x;
	merge(m * currentCorner);
	// max max max
	currentCorner.z = oldMax.z;
	merge(m * currentCorner);
	// max min max
	currentCorner.y = oldMin.y;
	merge(m * currentCorner);
	// max min min
	currentCorner.z = oldMin.z;
	merge(m * currentCorner);
}

} // namespace en
