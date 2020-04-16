#pragma once

#include <Enlivengine/Math/Matrix4.hpp>

namespace en 
{

class AABB;
class Plane;
class Ray;

class Sphere
{
	public:
		Sphere();
		Sphere(const Vector3f& center, F32 radius = 1.f);
		Sphere(F32 radius, const Vector3f& center = Vector3f::zero);
		
		const Vector3f& getCenter() const;
		void setCenter(const Vector3f& center);

		F32 getRadius() const;
		void setRadius(F32 radius);

		bool operator==(const Sphere& p) const;
		bool operator!=(const Sphere& p) const;

		F32 getDistance(const Vector3f& point) const;

		bool contains(const Vector3f& point) const;
		bool contains(const AABB& box) const;
		bool contains(const Sphere& sphere) const;
		// TODO : contains OBB
		// TODO : contains Frustum

		bool intersects(const AABB& box) const;
		bool intersects(const Sphere& sphere) const;
		bool intersects(const Plane& plane) const;
		bool intersects(const Ray& ray) const;
		// TODO : intersects OBB
		// TODO : intersects Frustum

		AABB getAABB() const;
		void fromAABB(const AABB& aabb);

		void merge(const Vector3f& point);
		// TODO : merge ?

		void transform(const Matrix4f& m);
		
	private:
		Vector3f mCenter;
		F32 mRadius;
};

} // namespace en
