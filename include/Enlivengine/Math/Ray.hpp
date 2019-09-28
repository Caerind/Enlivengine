#pragma once

#include <Enlivengine/Math/Vector3.hpp>

namespace en
{

class AABB;
class Sphere;
class Plane;

class Ray
{
	public:
		Ray();
		Ray(const Vector3f& origin, const Vector3f& direction);

		void setOrigin(const Vector3f& origin);
		void setOrigin(F32 x, F32 y, F32 z);
		const Vector3f& getOrigin() const;

		void setDirection(const Vector3f& direction);
		void setDirection(F32 x, F32 y, F32 z);
		const Vector3f& getDirection() const;

		Vector3f getPoint(F32 t) const;
		Vector3f operator*(F32 t) const;

		bool operator==(const Ray& ray) const;
		bool operator!=(const Ray& ray) const;

		bool contains(const Vector3f& point) const;

		bool intersects(const AABB& abox, F32* distance = nullptr) const;
		bool intersects(const Sphere& sphere, F32* distance = nullptr) const;
		bool intersects(const Plane& plane, F32* distance = nullptr) const;
		bool intersects(const Ray& ray, F32* length = nullptr, F32* lengthOther = nullptr) const;
		// TODO : intersects OBB
		// TODO : intersects Frustum

	private:
		Vector3f mOrigin;
		Vector3f mDirection;
};

} // namespace en
