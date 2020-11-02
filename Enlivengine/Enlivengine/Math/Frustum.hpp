#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Plane.hpp>

namespace en 
{

class Frustum
{
public:
	enum class Planes { Near = 0, Far, Top, Bottom, Right, Left };
	enum class Corners { FTL = 0, FTR, FBR, FBL, NTL, NTR, NBR, NBL };

	constexpr Frustum() = default;
	inline Frustum(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up, Math::Handedness handedness)
		: Frustum()
	{
		Build(fov, aspect, nearPlane, farPlane, eye, target, up, handedness);
	}

	inline void Build(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up, Math::Handedness handedness)
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
			mPlanes[static_cast<U32>(Planes::Bottom)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NBL)]);
			mPlanes[static_cast<U32>(Planes::Right)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NTR)]);
			mPlanes[static_cast<U32>(Planes::Left)].Set(mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::FBL)], mCorners[static_cast<U32>(Corners::FTL)]);
		}
		else
		{
			mPlanes[static_cast<U32>(Planes::Near)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NTR)]);
			mPlanes[static_cast<U32>(Planes::Far)].Set(mCorners[static_cast<U32>(Corners::FTR)], mCorners[static_cast<U32>(Corners::FBL)], mCorners[static_cast<U32>(Corners::FTL)]);
			mPlanes[static_cast<U32>(Planes::Top)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::FTR)], mCorners[static_cast<U32>(Corners::FTL)]);
			mPlanes[static_cast<U32>(Planes::Bottom)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::NBR)]);
			mPlanes[static_cast<U32>(Planes::Right)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NTR)], mCorners[static_cast<U32>(Corners::NBR)]);
			mPlanes[static_cast<U32>(Planes::Left)].Set(mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FBL)]);
		}
	}

	constexpr bool Contains(const Vector3f& point) const
	{
		for (U32 i = 0; i < 6; i++)
		{
			if (mPlanes[i].GetSide(point) == Plane::Side::Negative)
			{
				return false;
			}
		}
		return true;
	}

	constexpr const Plane& GetPlane(U32 index) const { return mPlanes[index]; }
	constexpr const Plane& GetPlane(Planes plane) const { return mPlanes[static_cast<U32>(plane)]; }

	constexpr const Vector3f& GetCorner(U32 index) const { return mCorners[index]; }
	constexpr const Vector3f& GetCorner(Corners corner) const { return mCorners[static_cast<U32>(corner)]; }

private:
	Plane mPlanes[6];
	Vector3f mCorners[8];
};

} // namespace en

#endif // ENLIVE_MODULE_MATH