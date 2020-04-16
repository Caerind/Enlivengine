#pragma once

#include <Enlivengine/Math/AABB.hpp>

namespace en 
{

class OBB
{
	public:
		OBB();
		OBB(const Vector3f& min, const Vector3f& max);
		OBB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ);
		OBB(const AABB& box);
		OBB(const Vector3f& min, const Vector3f& max, const Matrix4f& matrix);
		OBB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ, const Matrix4f& matrix);
		OBB(const AABB& box, const Matrix4f& matrix);

		void set(const Vector3f& min, const Vector3f& max);
		void set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ);
		void set(const AABB& box);
		void set(const Vector3f& min, const Vector3f& max, const Matrix4f& matrix);
		void set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ, const Matrix4f& matrix);
		void set(const AABB& box, const Matrix4f& matrix);

		void transform(const Matrix4f& matrix);
		void setTransform(const Matrix4f& matrix);
		const Matrix4f& getTransform() const;

		Vector3f getCenter() const;

		/*
		   1-------2
		  /|      /|
		 / |     / |
		6-------7  |
		|  0----|--3
		| /     | /
		|/      |/
		5-------4
		*/
		Vector3f getCorner(U8 index) const;

		bool contains(const Vector3f& point) const;

		bool operator==(const OBB& box) const;
		bool operator!=(const OBB& box) const;

	private:
		AABB mLocalBox;
		Vector3f mTransformedCorners[8];
		Matrix4f mTransformation;
};

} // namespace en
