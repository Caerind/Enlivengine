#include <Enlivengine/Math/Ray.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Sphere.hpp>
#include <Enlivengine/Math/Plane.hpp>

namespace en
{

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
	: mOrigin(origin)
	, mDirection(direction)
{
}

void Ray::setOrigin(const Vector3f& origin)
{
	mOrigin = origin;
}

void Ray::setOrigin(F32 x, F32 y, F32 z)
{
	setOrigin(Vector3f(x, y, z));
}

const Vector3f& Ray::getOrigin() const
{
	return mOrigin;
}

void Ray::setDirection(const Vector3f& direction)
{
	mDirection = direction;
}

void Ray::setDirection(F32 x, F32 y, F32 z)
{
	setDirection(Vector3f(x, y, z));
}

const Vector3f& Ray::getDirection() const
{
	return mDirection;
}

Vector3f Ray::getPoint(F32 t) const
{
	return mOrigin + mDirection * t;
}

Vector3f Ray::operator*(F32 t) const
{
	return mOrigin + mDirection * t;
}

bool Ray::operator==(const Ray& ray) const
{
	return mOrigin == ray.mOrigin && mDirection == ray.mDirection;
}

bool Ray::operator!=(const Ray& ray) const
{
	return !operator==(ray);
}

bool Ray::contains(const Vector3f& point) const
{
	if (mOrigin == point)
	{
		return true;
	}
	F32 t = (point - mOrigin).GetLength();
	return (mOrigin + t * mDirection) == point;
}

bool Ray::intersects(const AABB& box, F32* distance) const
{
	enAssert(mDirection.x != 0.0f);
	enAssert(mDirection.y != 0.0f);
	enAssert(mDirection.z != 0.0f);
	Vector3f t1 = (box.getMinimum() - mOrigin) / mDirection;
	Vector3f t2 = (box.getMaximum() - mOrigin) / mDirection;
	Vector3f vmax = Vector3f::Maximum(t1, t2);
	Vector3f vmin = Vector3f::Minimum(t1, t2);
	F32 tmax = Math::Min(vmax.x, Math::Min(vmax.y, vmax.z));
	F32 tmin = Math::Min(vmin.x, Math::Min(vmin.y, vmin.z));
	if (distance != nullptr)
	{
		*distance = tmin;
	}
	return tmax > tmin && tmin > 0.0f;
}

bool Ray::intersects(const Sphere& sphere, F32* distance) const
{
	F32 i = mDirection.DotProduct(sphere.getCenter() - mOrigin);
	if (i < 0.0f)
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false;
	}
	Vector3f interior = mOrigin + mDirection * i;
	F32 lSquared = (sphere.getRadius() * sphere.getRadius()) - (interior - sphere.getCenter()).GetSquaredLength();
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
		*distance = i - sqrt(lSquared);
	}
	return true;
}

bool Ray::intersects(const Plane& plane, F32* distance) const
{
	F32 den = mDirection.DotProduct(plane.getNormal());
	if (Math::Equals(den, 0.0f))
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false;
	}
	F32 nom = mOrigin.DotProduct(plane.getNormal()) + plane.getConstant();
	F32 t = -(nom / den);
	if (distance != nullptr)
	{
		*distance = t;
	}
	return t >= 0.0f;
}

bool Ray::intersects(const Ray& ray, F32* length, F32* lengthOther) const
{
	const Vector3f d = ray.mOrigin - mOrigin;
	const F32 q2 = (mDirection.y * ray.mDirection.x - mDirection.x * ray.mDirection.y);
	if (Math::Equals(q2, 0.0f) || Math::Equals(mDirection.x, 0.0f))
	{
		if (length)
		{
			*length = 0.0f;
		}
		if (lengthOther)
		{
			*lengthOther = 0.0f;
		}
		return false;
	}

	const F32 t2 = (mDirection.x * d.y - mDirection.y * d.x) / q2;
	const F32 t1 = (ray.mDirection.x * t2 + d.x) / mDirection.x;

	if (Math::Equals(mDirection.z * t1 + mOrigin.z - (ray.mDirection.z * t2 + ray.mOrigin.z), 0.0f))
	{
		if (length)
		{
			*length = t1;
		}
		if (lengthOther)
		{
			*lengthOther = t2;
		}
		return true;
	}
	else
	{
		if (length)
		{
			*length = 0.0f;
		}
		if (lengthOther)
		{
			*lengthOther = 0.0f;
		}
		return false;
	}
}

} // namespace en
