#include <Enlivengine/Core/CameraComponent.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Core/TransformComponent.hpp>

namespace en
{

CameraComponent::CameraComponent()
	: Camera()
	, mEntity()
{
}

CameraComponent::~CameraComponent()
{
}

CameraComponent::CameraComponent(CameraComponent&& other) noexcept
	: Camera()
	, mEntity(other.mEntity)
{
	other.mEntity = Entity();
}

CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept
{
	mEntity = other.mEntity;
	other.mEntity = Entity();
	return *this;
}

void CameraComponent::Apply() const
{
	Camera::Apply();
	if (GetProjection() == Camera::ProjectionMode::Perspective)
	{
		bgfx::setViewTransform(GetViewID(), GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
	else
	{
		bgfx::setViewTransform(GetViewID(), GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
}

Frustum CameraComponent::CreateFrustum() const
{
	return Camera::CreateFrustum().Transform(GetEntityMatrix());
}

const Matrix4f& CameraComponent::GetViewMatrix() const
{
	const Matrix4f& entityMatrix = GetEntityMatrix();
	const Vector3f position = entityMatrix.TransformPoint(GetPosition());
	const Vector3f direction = entityMatrix.TransformDirection(GetRotation().GetForward());
	// TODO : Only if CameraDirty or EntityTransformDirty
	mViewMatrix = Matrix4f::LookAt(position, position + direction, ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	return mViewMatrix;
}

Entity CameraComponent::GetEntity() const
{
	return mEntity;
}

World* CameraComponent::GetWorld()
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

const World* CameraComponent::GetWorld() const
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

Matrix4f CameraComponent::GetEntityMatrix() const
{
	if (mEntity.IsValid() && mEntity.Has<TransformComponent>())
	{
		return mEntity.Get<TransformComponent>().GetGlobalMatrix();
	}
	else
	{
		return Matrix4f::Identity();
	}
}

} // namespace en

#endif // ENLIVE_MODULE_CORE