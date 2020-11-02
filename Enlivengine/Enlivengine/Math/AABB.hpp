#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector3.hpp>

namespace en 
{

class AABB
{
public:
	constexpr AABB() : mMin(), mMax() {}
	constexpr AABB(const Vector3f& min, const Vector3f& max) : mMin(min), mMax(max) {}
	constexpr AABB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ) : mMin(minX, minY, minZ), mMax(maxX, maxY, maxZ) {}

	constexpr const Vector3f& GetMin() const { return mMin; }
	constexpr void SetMin(const Vector3f& min) { mMin.Set(min); }

	constexpr const Vector3f& GetMax() const { return mMax; }
	constexpr void SetMax(const Vector3f& max) { mMax.Set(max); }

	constexpr void Set(const Vector3f& min, const Vector3f& max) { mMin = min; mMax = max; }
	constexpr void Set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ) { mMin.Set(minX, minY, minZ); mMax.Set(maxX, maxY, maxZ); }
	constexpr void SetCenteredBox(const Vector3f& center, const Vector3f& halfSize) { mMin = center - halfSize; mMax = center + halfSize; }
	constexpr void SetCenteredBox(F32 cX, F32 cY, F32 cZ, F32 hsX, F32 hsY, F32 hsZ) { mMin.Set(cX - hsX, cY - hsY, cZ - hsZ); mMax.Set(cX + hsX, cY + hsY, cZ + hsZ); }

	constexpr Vector3f GetCenter() const { return (mMin + mMax) * 0.5f; }
	constexpr Vector3f GetSize() const { return mMax - mMin; }
	constexpr Vector3f GetHalfSize() const { return (mMax - mMin) * 0.5f; }
	constexpr F32 GetVolume() const { const Vector3f size = GetSize(); return size.x * size.y * size.y; }

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
	constexpr Vector3f GetCorner(U32 index) const
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

	constexpr bool operator==(const AABB& box) const { return mMin == box.mMin && mMax == box.mMax; }
	constexpr bool operator!=(const AABB& box) const { return !operator==(box); }

	constexpr F32 GetDistanceSquared(const Vector3f& point) const
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
	inline F32 GetDistance(const Vector3f& point) const
	{
		return Math::FastSqrt(GetDistanceSquared(point));
	}

	constexpr bool Contains(const Vector3f& point) const
	{
		return point >= mMin && point <= mMax;
	}

	constexpr bool Contains(const AABB& box) const
	{
		return box.mMin >= mMin && box.mMax <= mMax;
	}

	constexpr bool Intersects(const AABB& box, AABB* intersection = nullptr) const
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

private:
	Vector3f mMin;
	Vector3f mMax;
};

} // namespace en

#endif // ENLIVE_MODULE_MATH