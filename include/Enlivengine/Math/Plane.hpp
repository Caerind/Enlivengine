#pragma once

#include <Enlivengine/Math/Matrix4.hpp>

namespace en
{

class AABB;
class Sphere;
class Ray;

class Plane
{
	public:
		Plane();
		Plane(const Vector3f& normal, F32 constant);
		Plane(const Vector3f& normal, const Vector3f& point);
		Plane(F32 a, F32 b, F32 c, F32 d);
		Plane(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3);

		void set(const Vector3f& normal, F32 constant);
		void set(const Vector3f& normal, const Vector3f& point);
		void set(F32 a, F32 b, F32 c, F32 d);
		void set(const Vector3f& point1, const Vector3f& point2, const Vector3f& point3);

		const Vector3f& getNormal() const;
		void setNormal(const Vector3f& normal);

		F32 getConstant() const;
		void setConstant(F32 constant);

		F32 normalize();
		Plane normalized() const;

		enum Side : U8
		{
			None,
			Positive,
			Negative,
			Both
		};
		Side getSide(const Vector3f& point) const;
		Side getSide(const AABB& box) const;
		Side getSide(const Sphere& sphere) const;
		Side getSide(const Ray& ray) const;

		Vector3f getClosestPoint(const Vector3f& point);

		bool operator==(const Plane& p) const;
		bool operator!=(const Plane& p) const;

		F32 getDistance(const Vector3f& point) const;

		bool contains(const Vector3f& point) const;
		bool contains(const Ray& ray) const;

		bool intersects(const AABB& box) const;
		bool intersects(const Sphere& sphere) const;
		bool intersects(const Plane& plane) const;
		bool intersects(const Ray& ray) const;
		// TODO : intersects OBB
		// TODO : intersects Frustum

		// TODO : transform ?

	private:
		Vector3f mNormal;
		F32 mConstant;
};

} // namespace en
