#include <Enlivengine/Core/Transform.hpp>

namespace en
{

Transform::Transform()
	: mMatrix()
	, mPosition(0.0f)
	, mScale(1.0f)
	, mRotation2D(0.0f)
	, mDirty(true)
{
}

Vector2f Transform::GetPosition2D() const
{
	return Vector2f(mPosition.x, mPosition.y);
}

const Vector3f& Transform::GetPosition() const
{
	return mPosition;
}

void Transform::SetPosition(const Vector2f& position)
{
	mPosition.Set(position, mPosition.z);
	mDirty = true;
}

void Transform::SetPosition(const Vector3f& position)
{
	mPosition.Set(position);
	mDirty = true;
}

void Transform::SetPosition(F32 x, F32 y)
{
	mPosition.Set(x, y, mPosition.z);
	mDirty = true;
}

void Transform::SetPosition(F32 x, F32 y, F32 z)
{
	mPosition.Set(x, y, z);
	mDirty = true;
}

void Transform::Move(const Vector2f& movement)
{
	mPosition.x += movement.x;
	mPosition.y += movement.y;
	mDirty = true;
}

void Transform::Move(const Vector3f& movement)
{
	mPosition += movement;
	mDirty = true;
}

void Transform::Move(F32 x, F32 y, F32 z /*= 0.0f*/)
{
	mPosition += Vector3f(x, y, z);
	mDirty = true;
}

F32 Transform::GetRotation2D() const
{
	return mRotation2D;
}

void Transform::SetRotation2D(F32 rotation)
{
	mRotation2D = rotation;
	mDirty = true;
}

void Transform::Rotate2D(F32 rotation)
{
	mRotation2D += rotation;
	mDirty = true;
}

bool Transform::HasUniformScale() const
{
	return Math::Equals(mScale.x, mScale.y) && Math::Equals(mScale.y, mScale.z);
}

F32 Transform::GetUniformScale() const
{
	enAssert(HasUniformScale());
	return mScale.x;
}

Vector2f Transform::GetScale2D() const
{
	return Vector2f(mScale.x, mScale.y);
}

const Vector3f& Transform::GetScale() const
{
	return mScale;
}

void Transform::SetScale(F32 uniformScale)
{
	mScale.Set(uniformScale);
	mDirty = true;
}

void Transform::SetScale(const Vector2f& scale)
{
	mScale.Set(scale, mScale.z);
	mDirty = true;
}

void Transform::SetScale(const Vector3f& scale)
{
	mScale.Set(scale);
	mDirty = true;
}

void Transform::SetScale(F32 x, F32 y)
{
	mScale.Set(x, y, mScale.z);
	mDirty = true;
}

void Transform::SetScale(F32 x, F32 y, F32 z)
{
	mScale.Set(x, y, z);
	mDirty = true;
}

void Transform::Scale(F32 uniformScaleFactor)
{
	mScale *= Vector3(uniformScaleFactor);
	mDirty = true;
}

void Transform::Scale(const Vector2f& scaleFactor)
{
	mScale.x *= scaleFactor.x;
	mScale.y *= scaleFactor.y;
	mDirty = true;
}

void Transform::Scale(const Vector3f& scaleFactor)
{
	mScale *= scaleFactor;
	mDirty = true;
}

void Transform::Scale(F32 x, F32 y, F32 z /*= 1.0f*/)
{
	mScale *= Vector3(x, y, z);
	mDirty = true;
}

const Matrix4f& Transform::GetMatrix() const
{
	if (mDirty)
	{
		mMatrix.set(Matrix4f::identity);
		const F32 cos = Math::Cos(mRotation2D);
		const F32 sin = Math::Sin(mRotation2D);
		mMatrix[0] = mScale.x * cos;
		mMatrix[1] = mScale.y * -sin;
		mMatrix[4] = mScale.x * sin;
		mMatrix[5] = mScale.y * cos;
		mMatrix[10] = mScale.z;
		mMatrix[12] = mPosition.x;
		mMatrix[13] = mPosition.y;
		mMatrix[14] = mPosition.z;
		mDirty = false;
	}
	return mMatrix;
}

} // namespace en