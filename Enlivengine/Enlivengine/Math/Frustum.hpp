#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>

namespace en 
{

class AABB;
class Frustum;
class Plane;
class Ray;
class Sphere;

class Frustum
{
public:
	enum class Planes { Near = 0, Far, Top, Bottom, Right, Left };
	enum class Corners { FTL = 0, FTR, FBR, FBL, NTL, NTR, NBR, NBL };

	Frustum();
	Frustum(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up = ENLIVE_DEFAULT_UP, Math::Handedness handedness = ENLIVE_DEFAULT_HANDEDNESS);
	Frustum(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up = ENLIVE_DEFAULT_UP, Math::Handedness handedness = ENLIVE_DEFAULT_HANDEDNESS);

	void Build(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up = ENLIVE_DEFAULT_UP, Math::Handedness handedness = ENLIVE_DEFAULT_HANDEDNESS);
	void Build(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane, const Vector3f& eye, const Vector3f& target, const Vector3f& up = ENLIVE_DEFAULT_UP, Math::Handedness handedness = ENLIVE_DEFAULT_HANDEDNESS);

	bool Contains(const Vector3f& point) const;
	bool Contains(const AABB& aabb) const;
	bool Contains(const Frustum& frustum) const;
	bool Contains(const Sphere& sphere) const;

	bool Intersects(const AABB& aabb) const;
	bool Intersects(const Frustum& frustum) const;
	bool Intersects(const Plane& plane) const;
	bool Intersects(const Ray& ray) const;
	bool Intersects(const Sphere& sphere) const;

	const Plane& GetPlane(U32 index) const;
	const Plane& GetPlane(Planes plane) const;

	const Vector3f& GetCorner(U32 index) const;
	const Vector3f& GetCorner(Corners corner) const;

	Frustum Transform(const Matrix4f& transform) const;

private:
	void BuildPlanesFromCorners(Math::Handedness handedness);

private:
	Plane mPlanes[6];
	Vector3f mCorners[8];
};

} // namespace en

#endif // ENLIVE_MODULE_MATH