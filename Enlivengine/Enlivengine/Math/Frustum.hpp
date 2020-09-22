#pragma once

#include <Enlivengine/Math/Plane.hpp>

namespace en 
{
	
class AABB;
class Sphere;

class Frustum
{
	public:
		Frustum();
		Frustum(F32 fov, F32 ratio, F32 zNear, F32 zFar, const Vector3f& eye, const Vector3f& target, const Vector3f& up);

		enum Planes { Near = 0, Far, Top, Bottom, Right, Left };
		enum Corners { FTL = 0, FTR, FBR, FBL, NTL, NTR, NBR, NBL };

		void build(F32 fov, F32 ratio, F32 zNear, F32 zFar, const Vector3f& eye, const Vector3f& target, const Vector3f& up);

		bool contains(const Vector3f& point) const;
		bool contains(const AABB& box) const;
		bool contains(const Sphere& sphere) const;

		const Plane& getPlane(U32 index) const;
		const Vector3f& getCorner(U32 index) const;

	private:
		Plane mPlanes[6];
		Vector3f mCorners[8];
};

} // namespace en
