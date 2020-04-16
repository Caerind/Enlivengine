#include <Enlivengine/Math/Frustum.hpp>

namespace en
{

Frustum::Frustum()
{
}

Frustum::Frustum(F32 fov, F32 ratio, F32 zNear, F32 zFar, const Vector3f& eye, const Vector3f& target, const Vector3f& up)
{
	build(fov, ratio, zNear, zFar, eye, target, up);
}

void Frustum::build(F32 fov, F32 ratio, F32 zNear, F32 zFar, const Vector3f& eye, const Vector3f& target, const Vector3f& up)
{
	fov *= 0.5f;
	F32 tangent = Math::Tan(fov);
	F32 nearH = zNear * tangent;
	F32 nearW = nearH * ratio;
	F32 farH = zFar * tangent;
	F32 farW = farH * ratio;

	Vector3f f((target - eye).normalized());
	Vector3f s(up.crossProduct(f).normalized());
	Vector3f u(f.crossProduct(s));

	Vector3f nc = eye + f * zNear;
	Vector3f fc = eye + f * zFar;

	mCorners[Corners::FTL] = fc + u * farH - s * farW;
	mCorners[Corners::FTR] = fc + u * farH + s * farW;
	mCorners[Corners::FBR] = fc - u * farH + s * farW;
	mCorners[Corners::FBL] = fc - u * farH - s * farW;
	mCorners[Corners::NTL] = nc + u * nearH - s * nearW;
	mCorners[Corners::NTR] = nc + u * nearH + s * nearW;
	mCorners[Corners::NBR] = nc - u * nearH + s * nearW;
	mCorners[Corners::NBL] = nc - u * nearH - s * nearW; 

	mPlanes[Planes::Near].set(mCorners[Corners::NTL], mCorners[Corners::NTR], mCorners[Corners::NBR]);
	mPlanes[Planes::Far].set(mCorners[Corners::FTR], mCorners[Corners::FTL], mCorners[Corners::FBL]);
	mPlanes[Planes::Top].set(mCorners[Corners::NTL], mCorners[Corners::FTL], mCorners[Corners::FTR]);
	mPlanes[Planes::Bottom].set(mCorners[Corners::FBR], mCorners[Corners::NBR], mCorners[Corners::NBL]);
	mPlanes[Planes::Right].set(mCorners[Corners::FBR], mCorners[Corners::NBR], mCorners[Corners::NTR]);
	mPlanes[Planes::Left].set(mCorners[Corners::NBL], mCorners[Corners::FBL], mCorners[Corners::FTL]);
}

bool Frustum::contains(const Vector3f& point) const
{
	for (U32 i = 0; i < 6; i++)
	{
		if (mPlanes[i].getSide(point) == Plane::Side::Negative)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::contains(const AABB& box) const
{
	for (U32 i = 0; i < 6; i++)
	{
		if (mPlanes[i].getSide(box) == Plane::Side::Negative)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::contains(const Sphere& sphere) const
{
	for (U32 i = 0; i < 6; i++)
	{
		if (mPlanes[i].getSide(sphere) == Plane::Side::Negative)
		{
			return false;
		}
	}
	return true;
}

const Plane& Frustum::getPlane(U32 index) const
{
	return mPlanes[index];
}

const Vector3f& Frustum::getCorner(U32 index) const
{
	return mCorners[index];
}

} // namespace en
