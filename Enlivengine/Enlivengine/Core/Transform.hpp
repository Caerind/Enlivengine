#pragma once

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>

// TODO : 3D Rotation

namespace en
{

class Transform
{
public:
	Transform();

	// Position
	Vector2f GetPosition2D() const;
	const Vector3f& GetPosition() const;
	void SetPosition(const Vector2f& position);
	void SetPosition(const Vector3f& position);
	void SetPosition(F32 x, F32 y);
	void SetPosition(F32 x, F32 y, F32 z);
	void Move(const Vector2f& movement);
	void Move(const Vector3f& movement);
	void Move(F32 x, F32 y, F32 z = 0.0f);

	// Rotation
	F32 GetRotation2D() const;
	void SetRotation2D(F32 rotation);
	void Rotate2D(F32 rotation);

	// Scale
	bool HasUniformScale() const;
	F32 GetUniformScale() const;
	Vector2f GetScale2D() const;
	const Vector3f& GetScale() const;
	void SetScale(F32 uniformScale);
	void SetScale(const Vector2f& scale);
	void SetScale(const Vector3f& scale);
	void SetScale(F32 x, F32 y);
	void SetScale(F32 x, F32 y, F32 z);
	void Scale(F32 uniformScaleFactor);
	void Scale(const Vector2f& scaleFactor);
	void Scale(const Vector3f& scaleFactor);
	void Scale(F32 x, F32 y, F32 z = 1.0f);

	// Matrix
	const Matrix4f& GetMatrix() const;

private:
	mutable Matrix4f mMatrix;
	Vector3f mPosition;
	Vector3f mScale;
	F32 mRotation2D;
	mutable bool mDirty;
};

static_assert(sizeof(Transform) <= 128);

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Transform)
	ENLIVE_META_CLASS_MEMBER("Position", &en::Transform::GetPosition, &en::Transform::SetPosition),
	ENLIVE_META_CLASS_MEMBER("Rotation", &en::Transform::GetRotation2D, &en::Transform::SetRotation2D),
	ENLIVE_META_CLASS_MEMBER("Scale", &en::Transform::GetScale, &en::Transform::SetScale)
ENLIVE_META_CLASS_END()
