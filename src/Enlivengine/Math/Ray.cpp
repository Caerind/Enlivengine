#include <Enlivengine/Math/Ray.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Sphere.hpp>

namespace en 
{

Ray::Ray()
	: mOrigin()
	, mDirection(Vector3f::UnitX())
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
	: mOrigin(origin)
	, mDirection(direction)
{
}

const Vector3f& Ray::GetOrigin() const
{
	return mOrigin;
}

void Ray::SetOrigin(const Vector3f& origin)
{
	mOrigin = origin;
}

const Vector3f& Ray::GetDirection() const
{
	return mDirection;
}

void Ray::SetDirection(const Vector3f& direction)
{
	mDirection = direction;
}

bool Ray::operator==(const Ray& other) const
{
	return mOrigin == other.mOrigin && mDirection == other.mDirection;
}

bool Ray::operator!=(const Ray& other) const
{
	return !operator==(other);
}

Vector3f Ray::GetPoint(F32 t) const
{
	return mOrigin + t * mDirection;
}

bool Ray::Contains(const Vector3f& point) const
{
	const F32 t = (point - mOrigin).GetLength();
	return (mOrigin + t * mDirection) == point;
}

bool Ray::Intersects(const AABB& aabb, F32* distance /*= nullptr*/) const
{
	enAssert(!mDirection.IsZero());
	const Vector3f t1 = (aabb.GetMin() - mOrigin) / mDirection;
	const Vector3f t2 = (aabb.GetMax() - mOrigin) / mDirection;
	const Vector3f vmax(Math::Max(t1.x, t2.x), Math::Max(t1.y, t2.y), Math::Max(t1.z, t2.z));
	const Vector3f vmin(Math::Min(t1.x, t2.x), Math::Min(t1.y, t2.y), Math::Min(t1.z, t2.z));
	const F32 tmax = Math::Min(vmax.x, Math::Min(vmax.y, vmax.z));
	const F32 tmin = Math::Min(vmin.x, Math::Min(vmin.y, vmin.z));
	const bool result = tmax > tmin && tmin > 0.0f;
	if (distance != nullptr && result)
	{
		*distance = result ? tmin : 0.0f;
	}
	return result;
}

bool Ray::Intersects(const Frustum& frustum, F32* distance /*= nullptr*/) const
{ 
	bool result = false;
	F32 distanceMin = 99999.9f;
	for (U32 i = 0; i < 6; ++i)
	{
		F32 distanceTmp;
		if (Intersects(frustum.GetPlane(i), &distanceTmp))
		{
			result = true;
			distanceMin = Math::Min(distanceMin, distanceTmp);
		}
	}
	if (distance != nullptr)
	{
		*distance = (result) ? distanceMin : 0.0f;
	}
	return result;
}

bool Ray::Intersects(const Plane& plane, F32* distance /*= nullptr*/) const
{
	const F32 den = mDirection.Dot(plane.GetNormal());
	if (Math::Equals(den, 0.0f))
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false;
	}
	const F32 nom = mOrigin.Dot(plane.GetNormal()) + plane.GetConstant();
	const F32 t = -(nom / den);
	const bool result = t >= 0.0f;
	if (distance != nullptr)
	{
		*distance = result ? t : 0.0f;
	}
	return result;
}

bool Ray::Intersects(const Ray& ray, F32* distance /*= nullptr*/, F32* distanceOther /*=nullptr*/) const
{
	const Vector3f d = ray.mOrigin - mOrigin;
	const F32 q2 = (mDirection.y * ray.mDirection.x - mDirection.x * ray.mDirection.y);
	if (Math::Equals(q2, 0.0f) || Math::Equals(mDirection.x, 0.0f))
	{
		if (distance)
		{
			*distance = 0.0f;
		}
		if (distanceOther)
		{
			*distanceOther = 0.0f;
		}
		return false;
	}

	const F32 t2 = (mDirection.x * d.y - mDirection.y * d.x) / q2;
	const F32 t1 = (ray.mDirection.x * t2 + d.x) / mDirection.x;

	if (Math::Equals(mDirection.z * t1 + mOrigin.z - (ray.mDirection.z * t2 + ray.mOrigin.z), 0.0f))
	{
		if (distance)
		{
			*distance = t1;
		}
		if (distanceOther)
		{
			*distanceOther = t2;
		}
		return true;
	}
	else
	{
		if (distance)
		{
			*distance = 0.0f;
		}
		if (distanceOther)
		{
			*distanceOther = 0.0f;
		}
		return false;
	}
}

bool Ray::Intersects(const Sphere& sphere, F32* distance /*= nullptr*/) const
{
	const F32 i = mDirection.Dot(sphere.GetCenter() - mOrigin);
	if (i < 0.0f)
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false;
	}
	const Vector3f interior = mOrigin + mDirection * i;
	const F32 lSquared = (sphere.GetRadius() * sphere.GetRadius()) - (interior - sphere.GetCenter()).GetSquaredLength();
	if (lSquared < 0.0f)
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false;
	}
	if (distance != nullptr)
	{
		*distance = i - Math::FastSqrt(lSquared);
	}
	return true;
}

Ray Ray::Transform(const Matrix4f& transform) const
{
	return Ray(transform.TransformPoint(mOrigin), transform.TransformDirection(mDirection));
}

} // namespace en
