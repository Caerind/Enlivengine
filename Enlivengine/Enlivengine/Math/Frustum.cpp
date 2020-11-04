#include <Enlivengine/Math/Frustum.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>
#include <Enlivengine/Math/Sphere.hpp>

namespace en 
{

Frustum::Frustum()
	: mPlanes()
	, mCorners()
{
}

Frustum::Frustum(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up /*= ENLIVE_DEFAULT_UP*/, Math::Handedness handedness /*= ENLIVE_DEFAULT_HANDEDNESS*/)
	: Frustum()
{
	Build(fov, aspect, nearPlane, farPlane, eye, target, up, handedness);
}

void Frustum::Build(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up /*= ENLIVE_DEFAULT_UP*/, Math::Handedness handedness /*= ENLIVE_DEFAULT_HANDEDNESS*/)
{
	const F32 tangent = Math::Tan(fov * 0.5f);
	const F32 nearH = nearPlane * tangent;
	const F32 nearW = nearH * aspect;
	const F32 farH = farPlane * tangent;
	const F32 farW = farH * aspect;

	const Vector3f f((target - eye).Normalized());
	const Vector3f s(handedness == Math::Handedness::Right ? Vector3f::CrossProduct(f, up).Normalized() : Vector3f::CrossProduct(up, f).Normalized());
	const Vector3f u(handedness == Math::Handedness::Right ? Vector3f::CrossProduct(s, f) : Vector3f::CrossProduct(f, s));

	const Vector3f nc(eye + f * nearPlane);
	const Vector3f fc(eye + f * farPlane);

	mCorners[static_cast<U32>(Corners::FTL)] = fc + u * farH - s * farW;
	mCorners[static_cast<U32>(Corners::FTR)] = fc + u * farH + s * farW;
	mCorners[static_cast<U32>(Corners::FBR)] = fc - u * farH + s * farW;
	mCorners[static_cast<U32>(Corners::FBL)] = fc - u * farH - s * farW;
	mCorners[static_cast<U32>(Corners::NTL)] = nc + u * nearH - s * nearW;
	mCorners[static_cast<U32>(Corners::NTR)] = nc + u * nearH + s * nearW;
	mCorners[static_cast<U32>(Corners::NBR)] = nc - u * nearH + s * nearW;
	mCorners[static_cast<U32>(Corners::NBL)] = nc - u * nearH - s * nearW;

	if (handedness == Math::Handedness::Left)
	{
		mPlanes[static_cast<U32>(Planes::Near)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::NTR)], mCorners[static_cast<U32>(Corners::NBR)]);
		mPlanes[static_cast<U32>(Planes::Far)].Set(mCorners[static_cast<U32>(Corners::FTR)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FBL)]);
		mPlanes[static_cast<U32>(Planes::Top)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FTR)]);
		mPlanes[static_cast<U32>(Planes::Bottom)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::NBR)]);
		mPlanes[static_cast<U32>(Planes::Right)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NTR)], mCorners[static_cast<U32>(Corners::NBR)]);
		mPlanes[static_cast<U32>(Planes::Left)].Set(mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FBL)]);
	}
	else
	{
		mPlanes[static_cast<U32>(Planes::Near)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NTR)]);
		mPlanes[static_cast<U32>(Planes::Far)].Set(mCorners[static_cast<U32>(Corners::FTR)], mCorners[static_cast<U32>(Corners::FBL)], mCorners[static_cast<U32>(Corners::FTL)]);
		mPlanes[static_cast<U32>(Planes::Top)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::FTR)], mCorners[static_cast<U32>(Corners::FTL)]);
		mPlanes[static_cast<U32>(Planes::Bottom)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NBL)]);
		mPlanes[static_cast<U32>(Planes::Right)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NTR)], mCorners[static_cast<U32>(Corners::NBR)]);
		mPlanes[static_cast<U32>(Planes::Left)].Set(mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FBL)]);
	}
}

bool Frustum::Contains(const Vector3f& point) const
{
	for (U32 i = 0; i < 6; i++)
	{
		if (mPlanes[i].GetSide(point) != Plane::Side::Negative)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::Contains(const AABB& aabb) const
{ 
	for (U32 i = 0; i < 8; i++)
	{
		if (!Contains(aabb.GetCorner(i)))
		{
			return false;
		}
	}
	return true;
}

bool Frustum::Contains(const Frustum& frustum) const
{
	for (U32 i = 0; i < 8; i++)
	{
		if (!Contains(frustum.GetCorner(i)))
		{
			return false;
		}
	}
	return true;
}

bool Frustum::Contains(const Sphere& sphere) const
{
	AABB sphereAABB;
	sphereAABB.SetCenteredBox(sphere.GetCenter(), Vector3f(sphere.GetRadius()));
	return Contains(sphereAABB);
}

bool Frustum::Intersects(const AABB& aabb) const
{
	for (U32 i = 0; i < 6; ++i)
	{
		if (mPlanes[i].Intersects(aabb))
		{
			return true;
		}
	}
	return false;
}

bool Frustum::Intersects(const Frustum& frustum) const
{
	for (U32 i = 0; i < 6; ++i)
	{
		if (mPlanes[i].Intersects(frustum))
		{
			return true;
		}
	}
	return false;
}

bool Frustum::Intersects(const Plane& plane) const
{
	for (U32 i = 0; i < 6; ++i)
	{
		if (mPlanes[i].Intersects(plane))
		{
			return true;
		}
	}
	return false;
}

bool Frustum::Intersects(const Ray& ray) const
{
	for (U32 i = 0; i < 6; ++i)
	{
		if (mPlanes[i].Intersects(ray))
		{
			return true;
		}
	}
	return false;
}

bool Frustum::Intersects(const Sphere& sphere) const
{
	for (U32 i = 0; i < 6; ++i)
	{
		if (mPlanes[i].Intersects(sphere))
		{
			return true;
		}
	}
	return false;
}

const Plane& Frustum::GetPlane(U32 index) const
{
	return mPlanes[index];
}

const Plane& Frustum::GetPlane(Planes plane) const
{
	return mPlanes[static_cast<U32>(plane)];
}

const Vector3f& Frustum::GetCorner(U32 index) const
{
	return mCorners[index];
}

const Vector3f& Frustum::GetCorner(Corners corner) const
{
	return mCorners[static_cast<U32>(corner)];
}

Frustum Frustum::Transform(const Matrix4f& transform) const
{
	Frustum out;
	for (U32 i = 0; i < 6; ++i)
	{
		out.mPlanes[i] = mPlanes[i].Transform(transform);
	}
	for (U32 i = 0; i < 8; ++i)
	{
		out.mCorners[i] = transform.TransformPoint(mCorners[i]);
	}
	return out;
}

} // namespace en

#endif // ENLIVE_MODULE_MATH
