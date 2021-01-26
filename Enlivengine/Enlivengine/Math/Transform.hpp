#pragma once

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>

namespace en
{

class Transform
{
public:
	Transform();

	// Position
	Vector3f GetPosition() const;
	void SetPosition(const Vector3f& position);
	void Move(const Vector3f& movement);

	// Rotation
	Matrix3f GetRotation() const;
	void SetRotation(const Matrix3f& rotation);
	void Rotate(const Matrix3f& rotation);

	// Scale
	Vector3f GetScale() const;
	void SetScale(const Vector3f& scale);
	void SetUniformScale(F32 uniformScale);
	void Scale(const Vector3f& scale);
	void Scale(F32 uniformScale);

	// Transform
	void SetTransform(const Vector3f& translation, const Matrix3f& rotation, const Vector3f& scale = Vector3f(1.0f));

	const Matrix4f& GetMatrix() const;

protected:
	Matrix4f mMatrix;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Transform, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("position", &en::Transform::GetPosition, &en::Transform::SetPosition),
	ENLIVE_META_CLASS_MEMBER("rotation", &en::Transform::GetRotation, &en::Transform::SetRotation),
	ENLIVE_META_CLASS_MEMBER("scale", &en::Transform::GetScale, &en::Transform::SetScale)
ENLIVE_META_CLASS_END()
