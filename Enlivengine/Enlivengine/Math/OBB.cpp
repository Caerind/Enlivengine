#include <Enlivengine/Math/OBB.hpp>

namespace en
{

OBB::OBB()
{
	mTransformation.makeIdentity();
}

OBB::OBB(const Vector3f& min, const Vector3f& max)
{
	mTransformation.makeIdentity();
	set(min, max);
}

OBB::OBB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ)
{
	mTransformation.makeIdentity();
	set(minX, minY, minZ, maxX, maxY, maxZ);
}

OBB::OBB(const AABB& box)
{
	mTransformation.makeIdentity();
	set(box);
}

OBB::OBB(const Vector3f& min, const Vector3f& max, const Matrix4f& matrix)
{
	mTransformation.makeIdentity();
	set(min, max, matrix);
}

OBB::OBB(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ, const Matrix4f& matrix)
{
	mTransformation.makeIdentity();
	set(minX, minY, minZ, maxX, maxY, maxZ, matrix);
}

OBB::OBB(const AABB& box, const Matrix4f& matrix)
{
	mTransformation.makeIdentity();
	set(box, matrix);
}

void OBB::set(const Vector3f& min, const Vector3f& max)
{
	set(AABB(min, max));
}

void OBB::set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ)
{
	set(Vector3f(minX, minY, minZ), Vector3f(maxX, maxY, maxZ));
}

void OBB::set(const AABB& box)
{
	mLocalBox = box;
}

void OBB::set(const Vector3f& min, const Vector3f& max, const Matrix4f& matrix)
{
	set(min, max);
	transform(matrix);
}

void OBB::set(F32 minX, F32 minY, F32 minZ, F32 maxX, F32 maxY, F32 maxZ, const Matrix4f& matrix)
{
	set(minX, minY, minZ, maxX, maxY, maxZ);
	transform(matrix);
}

void OBB::set(const AABB& box, const Matrix4f& matrix)
{
	set(box);
	transform(matrix);
}

void OBB::transform(const Matrix4f& matrix)
{
	mTransformation *= matrix;
	for (U8 i = 0; i < 8; i++)
	{
		mTransformedCorners[i] = mTransformation * mLocalBox.getCorner(i);
	}
}

void OBB::setTransform(const Matrix4f& matrix)
{
	mTransformation = matrix;
	for (U8 i = 0; i < 8; i++)
	{
		mTransformedCorners[i] = mTransformation * mLocalBox.getCorner(i);
	}
}

const Matrix4f& OBB::getTransform() const
{
	return mTransformation;
}

Vector3f OBB::getCenter() const
{
	Vector3f result;
	for (U32 i = 0; i < 8; i++)
	{
		result += mTransformedCorners[i];
	}
	return result / 8.0f;
}

Vector3f OBB::getCorner(U8 index) const
{
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
	enAssert(index < 8);
	return mTransformedCorners[index]; 
}

bool OBB::contains(const Vector3f& point) const
{
	// TODO : Improve ?
	bool valid = false;
	Matrix4f inv = mTransformation.inversed(&valid);
	if (!valid)
	{
		return false;
	}
	return mLocalBox.contains(inv * point);
}

bool OBB::operator==(const OBB& box) const
{
	// TODO : Improve ?
	return mLocalBox == box.mLocalBox && mTransformation == box.mTransformation;
}

bool OBB::operator!=(const OBB& box) const
{
	return !operator==(box);
}

} // namespace en
