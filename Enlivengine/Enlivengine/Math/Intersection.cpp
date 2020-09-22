#include <Enlivengine/Math/Intersection.hpp>

namespace en
{

bool intersects(const AABB & a1, const AABB & a2, AABB * intersection)
{
	Vector3f min1 = a1.getMinimum();
	Vector3f max1 = a1.getMaximum();
	Vector3f min2 = a2.getMinimum();
	Vector3f max2 = a2.getMaximum();

	F32 left = Math::Max(min1.x, min2.x);
	F32 right = Math::Min(max1.x, max2.x);

	if (left >= right)
		return false;

	F32 top = Math::Max(min1.y, min2.y);
	F32 bottom = Math::Min(max1.y, max2.y);
	if (top >= bottom)
		return false;

	F32 up = Math::Max(min1.z, min2.z);
	F32 down = Math::Min(max1.z, max2.z);
	if (up >= down)
		return false;

	if (intersection)
	{
		intersection->setBox(left, top, up, right, bottom, down);
	}

	return true;
}

bool intersects(const AABB & a1, const Sphere & a2)
{
	return intersects(a2, a1);
}

bool intersects(const AABB & a1, const Plane & a2)
{
	return intersects(a2, a1);
}

bool intersects(const AABB & a1, const OBB & a2)
{
	return intersects(a2, a1);
}

bool intersects(const AABB & a1, const Ray & a2, F32 * distance)
{
	return intersects(a2, a1, distance);
}

bool intersects(const AABB & a1, const Frustum & a2)
{
	return intersects(a2, a1);
}

bool intersects(const Sphere & a1, const AABB & a2)
{
	// Use splitting planes
	const Vector3f& min(a2.getMinimum());
	const Vector3f& max(a2.getMaximum());
	const Vector3f& center(a1.getCenter());
	// Arvo's algorithm
	F32 s;
	F32 d = 0.f;
	for (U8 i = 0; i < 3; ++i)
	{
		if (center[i] < min[i])
		{
			s = center[i] - min[i];
			d += s * s;
		}
		else if (center[i] > max[i])
		{
			s = center[i] - max[i];
			d += s * s;
		}
	}
	return d <= a1.getRadius() * a1.getRadius();
}

bool intersects(const Sphere & a1, const Sphere & a2)
{
	return (a1.getCenter() - a2.getCenter()).GetSquaredLength() <= (a1.getRadius() + a2.getRadius()) * (a1.getRadius() + a2.getRadius());
}

bool intersects(const Sphere & a1, const Plane & a2)
{
	return intersects(a2, a1);
}

bool intersects(const Sphere & a1, const OBB & a2)
{
	return intersects(a2, a1);
}

bool intersects(const Sphere & a1, const Ray & a2, F32 * distance)
{
	return intersects(a2, a1, distance);
}

bool intersects(const Sphere & a1, const Frustum & a2)
{
	return intersects(a2, a1);
}

bool intersects(const Plane & a1, const AABB & a2)
{
	return getPlaneSide(a1, a2) == Plane::Side::Both;
}

bool intersects(const Plane & a1, const Sphere & a2)
{
	return fabs(a1.getDistance(a2.getCenter())) <= a2.getRadius();
}

bool intersects(const Plane & a1, const Plane & a2)
{
	if (a1 == a2)
	{
		return true;
	}
	Vector3f n1 = a1.getNormal().Normalized();
	Vector3f n2 = a2.getNormal().Normalized();
	if (n1 == n2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool intersects(const Plane & a1, const OBB & a2)
{
	return intersects(a2, a1);
}

bool intersects(const Plane & a1, const Ray & a2, F32 * distance)
{
	return intersects(a2, a1, distance);
}

bool intersects(const Plane & a1, const Frustum & a2)
{
	return intersects(a2, a1);
}

bool intersects(const OBB & a1, const AABB & a2)
{
	// TODO : Improve ?

	// OBB Points in AABB (faster than the other loop)
	for (U8 i = 0; i < 8; i++)
	{
		if (a2.contains(a1.getCorner(i)))
		{
			return true;
		}
	}

	// AABB Points in OBB
	for (U8 i = 0; i < 8; i++)
	{
		if (a1.contains(a2.getCorner(i)))
		{
			return true;
		}
	}

	return false;
}

bool intersects(const OBB & a1, const Sphere & a2)
{
	// OBB Points in AABB (faster than the other loop)
	for (U8 i = 0; i < 8; i++)
	{
		if (a2.contains(a1.getCorner(i)))
		{
			return true;
		}
	}

	// TODO : Missing some cases

	return false;
}

bool intersects(const OBB & a1, const Plane & a2)
{
	Plane::Side startSide = a2.getSide(a1.getCorner(0));
	for (U8 i = 1; i < 8; i++)
	{
		if (startSide != a2.getSide(a1.getCorner(i)))
		{
			return true;
		}
	}
	return false;
}

bool intersects(const OBB & a1, const OBB & a2)
{
	for (U8 i = 0; i < 8; i++)
	{
		if (a2.contains(a1.getCorner(i)))
		{
			return true;
		}
		if (a1.contains(a2.getCorner(i)))
		{
			return true;
		}
	}
	return false;
}

bool intersects(const OBB & a1, const Ray & a2, F32 * distance)
{
	return intersects(a2, a1, distance);
}

bool intersects(const OBB & a1, const Frustum & a2)
{
	return intersects(a2, a1);
}

bool intersects(const Ray & a1, const AABB & a2, F32 * distance)
{
	F32 tfirst = F32(0.0f);

	Vector3f boxMin = a2.getMinimum();
	Vector3f boxMax = a2.getMaximum();

	for (unsigned int i = 0; i < 3; ++i)
	{
		F32 dir = a1.getDirection()[i];
		F32 ori = a1.getOrigin()[i];
		F32 max = boxMax[i];
		F32 min = boxMin[i];

		if (Math::Equals(dir, F32(0.0f)))
		{
			if (ori < max && ori > min)
				continue;

			return false;
		}

		F32 tmin = (min - ori) / dir;
		F32 tmax = (max - ori) / dir;
		if (tmin > tmax)
		{
			F32 ttemp = tmin;
			tmin = tmax;
			tmax = ttemp;
		}

		if (tmax < tfirst)
			return false;

		tfirst = Math::Max(tfirst, tmin);
	}

	if (distance)
		*distance = tfirst;

	return true;
}

bool intersects(const Ray & a1, const Sphere & a2, F32 * distance)
{
	// Adjust ray origin relative to sphere center
	const Vector3f& diff = a1.getOrigin() - a2.getCenter();
	F32 radius = a2.getRadius();

	// Check origin inside first
	if (diff.GetSquaredLength() <= radius * radius)
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return true;
	}

	// Build coeffs which can be used with std quadratic solver
	// ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
	const Vector3f& dir = a1.getDirection();
	const F32 a = dir.GetSquaredLength();
	const F32 b = 2 * diff.DotProduct(dir);
	const F32 c = diff.GetSquaredLength() - (radius * radius);
	const F32 d = (b * b) - (4.f * a * c); // Calc determinant
	if (d < 0.f)
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false; // No intersection
	}
	else
	{
		F32 t = (-b - sqrt(d)) / (2.f * a);
		if (t < 0.f)
		{
			t = (-b + sqrt(d)) / (2.f * a);
		}
		if (distance != nullptr)
		{
			*distance = t;
		}
		return true;
	}
}

bool intersects(const Ray & a1, const Plane & a2, F32 * distance)
{
	F32 denom = a2.getNormal().DotProduct(a1.getDirection());
	if (fabs(denom) < 1e-7f)
	{
		if (distance != nullptr)
		{
			*distance = 0.0f;
		}
		return false; // Parallel
	}
	else
	{
		F32 nom = a2.getNormal().DotProduct(a1.getOrigin()) + a2.getConstant();
		F32 t = -(nom / denom);
		if (distance != nullptr)
		{
			*distance = t;
		}
		return t >= 0.f; 
	}
}

bool intersects(const Ray & a1, const OBB & a2, F32 * distance)
{
	ENLIVE_UNUSED(a1);
	ENLIVE_UNUSED(a2);
	ENLIVE_UNUSED(distance);
	return false; // TODO : This
}

bool intersects(const Ray & a1, const Ray & a2, F32 * l1, F32 * l2)
{
	const Vector3f& o1 = a1.getOrigin();
	const Vector3f& o2 = a2.getOrigin();
	const Vector3f& d1 = a1.getDirection();
	const Vector3f& d2 = a2.getDirection();
	const Vector3f d = o2 - o1;

	const F32 q2 = (d1.y * d2.x - d1.x * d2.y);

	if (Math::Equals(q2, 0.0f) || Math::Equals(d1.x, 0.0f))
	{
		if (l1)
		{
			*l1 = 0.0f;
		}
		if (l2)
		{
			*l2 = 0.0f;
		}
		return false;
	}

	const F32 t2 = (d1.x * d.y - d1.y * d.x) / q2;
	const F32 t1 = (d2.x * t2 + d.x) / d1.x;

	if (Math::Equals(d1.z * t1 + o1.z - (d2.z * t2 + o2.z), 0.0f))
	{
		if (l1)
		{
			*l1 = t1;
		}
		if (l2)
		{
			*l2 = t2;
		}
		return true;
	}
	else
	{
		if (l1)
		{
			*l1 = 0.0f;
		}
		if (l2)
		{
			*l2 = 0.0f;
		}
		return false;
	}
}

bool intersects(const Ray & a1, const Frustum & a2, F32 * distance)
{
	ENLIVE_UNUSED(a1);
	ENLIVE_UNUSED(a2);
	ENLIVE_UNUSED(distance);
	return false; // TODO : This
}

bool intersects(const Frustum & a1, const AABB & a2)
{
	for (U8 i = 0; i < 6; i++)
	{
		if (getPlaneSide(a1.getPlane(i), a2) == Plane::Side::Negative)
		{ 
			return false;
		}
	}
	return true;
}

bool intersects(const Frustum & a1, const Sphere & a2)
{
	for (U8 i = 0; i < 6; i++)
	{
		if (getPlaneSide(a1.getPlane(i), a2) == Plane::Side::Negative)
		{
			return false;
		}
	}
	return true;
}

bool intersects(const Frustum & a1, const Plane & a2)
{
	ENLIVE_UNUSED(a1);
	ENLIVE_UNUSED(a2);
	return false; // TODO : This
}

bool intersects(const Frustum & a1, const OBB & a2)
{
	for (U8 i = 0; i < 6; i++)
	{
		if (getPlaneSide(a1.getPlane(i), a2) == Plane::Side::Negative)
		{
			return false;
		}
	}
	return true;
}

bool intersects(const Frustum & a1, const Ray & a2, F32 * distance)
{
	return intersects(a2, a1, distance);
}

bool intersects(const Frustum & a1, const Frustum & a2)
{
	ENLIVE_UNUSED(a1);
	ENLIVE_UNUSED(a2);
	return false; // TODO : This
}

Plane::Side getPlaneSide(const Plane & plane, const AABB & box)
{
	F32 dist = plane.getDistance(box.getCenter());
	F32 maxAbsDist = abs(plane.getNormal().DotProduct(box.getHalfSize()));
	if (dist < -maxAbsDist)
		return Plane::Side::Negative;
	if (dist > +maxAbsDist)
		return Plane::Side::Positive;
	return Plane::Side::Both;
}

Plane::Side getPlaneSide(const Plane & plane, const Sphere & sphere)
{
	F32 distance = plane.getDistance(sphere.getCenter());
	if (sphere.getRadius() > abs(distance))
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

Plane::Side getPlaneSide(const Plane & plane, const OBB & box)
{
	Plane::Side startSide = plane.getSide(box.getCorner(0));
	for (U8 i = 1; i < 8; i++)
	{
		if (startSide != plane.getSide(box.getCorner(i)))
		{
			return Plane::Side::Both;
		}
	}
	return startSide;
}

Plane::Side getPlaneSide(const Plane & plane, const Ray & ray)
{
	if (intersects(ray, plane))
	{
		return Plane::Side::Both;
	}
	else
	{
		return plane.getSide(ray.getOrigin());
	}
}

} // namespace en
