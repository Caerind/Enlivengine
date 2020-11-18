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

protected:
	Matrix4f mMatrix;
};

} // namespace en
