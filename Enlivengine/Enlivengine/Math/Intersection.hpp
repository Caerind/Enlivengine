#pragma once

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Sphere.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/OBB.hpp>
#include <Enlivengine/Math/Ray.hpp>
#include <Enlivengine/Math/Frustum.hpp>

// TODO : Intersection OBB - Sphere
// TODO : Intersection Ray - OBB
// TODO : Intersection Ray - Frustum
// TODO : Intersection Frustum - Plane
// TODO : Intersection Frustum - Frustum

namespace en
{

bool intersects(const AABB& a1, const AABB& a2, AABB* intersection = nullptr);
bool intersects(const AABB& a1, const Sphere& a2);
bool intersects(const AABB& a1, const Plane& a2);
bool intersects(const AABB& a1, const OBB& a2);
bool intersects(const AABB& a1, const Ray& a2, F32* distance = nullptr);
bool intersects(const AABB& a1, const Frustum& a2);

bool intersects(const Sphere& a1, const AABB& a2);
bool intersects(const Sphere& a1, const Sphere& a2);
bool intersects(const Sphere& a1, const Plane& a2);
bool intersects(const Sphere& a1, const OBB& a2);
bool intersects(const Sphere& a1, const Ray& a2, F32* distance = nullptr);
bool intersects(const Sphere& a1, const Frustum& a2);

bool intersects(const Plane& a1, const AABB& a2);
bool intersects(const Plane& a1, const Sphere& a2);
bool intersects(const Plane& a1, const Plane& a2);
bool intersects(const Plane& a1, const OBB& a2);
bool intersects(const Plane& a1, const Ray& a2, F32* distance = nullptr);
bool intersects(const Plane& a1, const Frustum& a2);

bool intersects(const OBB& a1, const AABB& a2);
bool intersects(const OBB& a1, const Sphere& a2);
bool intersects(const OBB& a1, const Plane& a2);
bool intersects(const OBB& a1, const OBB& a2);
bool intersects(const OBB& a1, const Ray& a2, F32* distance = nullptr);
bool intersects(const OBB& a1, const Frustum& a2);

bool intersects(const Ray& a1, const AABB& a2, F32* distance = nullptr);
bool intersects(const Ray& a1, const Sphere& a2, F32* distance = nullptr);
bool intersects(const Ray& a1, const Plane& a2, F32* distance = nullptr);
bool intersects(const Ray& a1, const OBB& a2, F32* distance = nullptr);
bool intersects(const Ray& a1, const Ray& a2, F32* l1 = nullptr, F32* l2 = nullptr);
bool intersects(const Ray& a1, const Frustum& a2, F32* distance = nullptr);

bool intersects(const Frustum& a1, const AABB& a2);
bool intersects(const Frustum& a1, const Sphere& a2);
bool intersects(const Frustum& a1, const Plane& a2);
bool intersects(const Frustum& a1, const OBB& a2);
bool intersects(const Frustum& a1, const Ray& a2, F32* distance = nullptr);
bool intersects(const Frustum& a1, const Frustum& a2);

Plane::Side getPlaneSide(const Plane& plane, const AABB& box);
Plane::Side getPlaneSide(const Plane& plane, const Sphere& sphere);
Plane::Side getPlaneSide(const Plane& plane, const OBB& box);
Plane::Side getPlaneSide(const Plane& plane, const Ray& ray);

} // namespace en
