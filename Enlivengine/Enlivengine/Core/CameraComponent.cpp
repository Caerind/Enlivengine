#include <Enlivengine/Core/CameraComponent.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Math/Transform.hpp>

namespace en
{

CameraComponent::CameraComponent()
	: mEntity()
{
}

CameraComponent::CameraComponent(CameraComponent&& other) noexcept
	: mEntity(other.mEntity)
{
	other.mEntity = Entity();
}

CameraComponent::~CameraComponent()
{
}

CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept
{
	mEntity = other.mEntity;
	other.mEntity = Entity();
	return *this;
}

void CameraComponent::Apply() const
{
	mCamera.Apply();
	if (mCamera.GetProjection() == Camera::ProjectionMode::Perspective)
	{
		bgfx::setViewTransform(mCamera.GetViewID(), GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
	else
	{
		bgfx::setViewTransform(mCamera.GetViewID(), GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
}

Frustum CameraComponent::CreateFrustum() const
{
	return mCamera.CreateFrustum().Transform(GetEntityMatrix());
}

void CameraComponent::SetProjection(Camera::ProjectionMode projection)
{
	mCamera.SetProjection(projection);
}

Camera::ProjectionMode CameraComponent::GetProjection() const
{
	return mCamera.GetProjection();
}

void CameraComponent::InitializePerspective(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane)
{
	mCamera.InitializePerspective(fov, aspect, nearPlane, farPlane);
}

void CameraComponent::InitializeOrthographic(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane)
{
	mCamera.InitializeOrthographic(left, top, right, bottom, nearPlane, farPlane);
}

void CameraComponent::SetNearPlane(F32 nearPlane)
{
	mCamera.SetNearPlane(nearPlane);
}

F32 CameraComponent::GetNearPlane() const
{
	return mCamera.GetNearPlane();
}

void CameraComponent::SetFarPlane(F32 farPlane)
{
	mCamera.SetFarPlane(farPlane);
}

F32 CameraComponent::GetFarPlane() const
{
	return mCamera.GetFarPlane();
}

void CameraComponent::SetFOV(F32 fov)
{
	mCamera.SetFOV(fov);
}

F32 CameraComponent::GetFOV() const
{
	return mCamera.GetFOV();
}

void CameraComponent::SetAspect(F32 aspect)
{
	mCamera.SetAspect(aspect);
}

F32 CameraComponent::GetAspect() const
{
	return mCamera.GetAspect();
}

void CameraComponent::SetLeft(F32 left)
{
	mCamera.SetLeft(left);
}

F32 CameraComponent::GetLeft() const
{
	return mCamera.GetLeft();
}

void CameraComponent::SetTop(F32 top)
{
	mCamera.SetTop(top);
}

F32 CameraComponent::GetTop() const
{
	return mCamera.GetTop();
}

void CameraComponent::SetRight(F32 right)
{
	mCamera.SetRight(right);
}

F32 CameraComponent::GetRight() const
{
	return mCamera.GetRight();
}

void CameraComponent::SetBottom(F32 bottom)
{
	mCamera.SetBottom(bottom);
}

F32 CameraComponent::GetBottom() const
{
	return mCamera.GetBottom();
}

const Matrix4f& CameraComponent::GetProjectionMatrix() const
{
	return mCamera.GetProjectionMatrix();
}

void CameraComponent::InitializeView(const Vector3f& position, const Matrix3f& rotation)
{
	mCamera.InitializeView(position, rotation);
}

void CameraComponent::SetPosition(const Vector3f& position)
{
	mCamera.SetPosition(position);
}

const Vector3f& CameraComponent::GetPosition() const
{
	return mCamera.GetPosition();
}

void CameraComponent::Move(const Vector3f& movement)
{
	mCamera.Move(movement);
}

void CameraComponent::SetRotation(const Matrix3f& rotation)
{
	mCamera.SetRotation(rotation);
}

const Matrix3f& CameraComponent::GetRotation() const
{
	return mCamera.GetRotation();
}

void CameraComponent::Rotate(const Matrix3f& rotation)
{
	mCamera.Rotate(rotation);
}

const Matrix4f& CameraComponent::GetViewMatrix() const
{
	const Matrix4f& entityMatrix = GetEntityMatrix();
	const Vector3f position = entityMatrix.TransformPoint(mCamera.GetPosition());
	const Vector3f direction = entityMatrix.TransformDirection(mCamera.GetRotation().GetForward());
	mViewMatrix = Matrix4f::LookAt(position, position + direction, ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	return mViewMatrix;
}

void CameraComponent::SetClearColor(const Color& clearColor)
{
	mCamera.SetClearColor(clearColor);
}

const Color& CameraComponent::GetClearColor() const
{
	return mCamera.GetClearColor();
}

void CameraComponent::SetViewport(const Rectf& viewport)
{
	mCamera.SetViewport(viewport);
}

const Rectf& CameraComponent::GetViewport() const
{
	return mCamera.GetViewport();
}

void CameraComponent::SetFramebuffer(bgfx::FrameBufferHandle framebuffer)
{
	mCamera.SetFramebuffer(framebuffer);
}

bgfx::FrameBufferHandle CameraComponent::GetFramebuffer() const
{
	return mCamera.GetFramebuffer();
}

bgfx::ViewId CameraComponent::GetViewID() const
{
	return mCamera.GetViewID();
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

const Matrix4f& CameraComponent::GetEntityMatrix() const
{
	if (mEntity.IsValid() && mEntity.Has<TransformComponent>())
	{
		return mEntity.Get<TransformComponent>().transform.GetMatrix();
	}
	else
	{
		static const Matrix4f staticIdentity = Matrix4f::Identity();
		return staticIdentity;
	}
}

} // namespace en

#endif // ENLIVE_MODULE_CORE