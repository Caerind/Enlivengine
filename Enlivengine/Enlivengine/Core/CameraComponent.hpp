#pragma once

#include <Enlivengine/Graphics/Camera.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

class World;

class CameraComponent : public Camera
{
public:
	CameraComponent();
	~CameraComponent();

	CameraComponent(CameraComponent&& other) noexcept;
	CameraComponent(const CameraComponent&) = delete;

	CameraComponent& operator=(CameraComponent&& other) noexcept;
	CameraComponent& operator=(const CameraComponent&) = delete;

	virtual void Apply() const override;

	virtual Frustum CreateFrustum() const override;

	virtual const Matrix4f& GetViewMatrix() const override;

	bool Initialize(const Entity& entity);

	bool Serialize(ClassSerializer& serializer, const char* name);

private:
	// Hide camera view transform, use entity transform
	void InitializeView(const Vector3f& position, const Matrix3f& rotation);
	void SetPosition(const Vector3f& position);
	const Vector3f& GetPosition() const;
	void Move(const Vector3f& movement);
	void SetRotation(const Matrix3f& rotation);
	const Matrix3f& GetRotation() const;
	void Rotate(const Matrix3f& rotation);

private:
	Matrix4f GetEntityMatrix() const;

private:
	friend struct ComponentHasCustomInitialization<CameraComponent>;

	Entity mEntity;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::CameraComponent, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()
ENLIVE_COMPONENT_CUSTOM_INITIALIZATION(en::CameraComponent)