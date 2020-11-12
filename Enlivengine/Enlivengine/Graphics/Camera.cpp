#include <Enlivengine/Graphics/Camera.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Graphics/BgfxWrapper.hpp>

namespace en
{

bgfx::ViewId Camera::sViewIdCounter = 0;

Camera::Camera()
	: mViewMatrix()
	, mProjectionMatrix()
	, mRotation()
	, mViewport(Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, mPosition()
	, mClearColor(U32(0x443355FF))
	, mFramebuffer(BGFX_INVALID_HANDLE)
	, mViewId(sViewIdCounter++)
	, mProjectionMode(ProjectionMode::Perspective)
	, mProjectionDirty(true)
	, mViewDirty(true)
{
}

Camera::Camera(Camera&& other) noexcept
{
	// TODO
	ENLIVE_UNUSED(other);
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	// TODO
	ENLIVE_UNUSED(other);
	return *this;
}

void Camera::Apply() const
{
	BgfxWrapper::SetCurrentView(mViewId);
	bgfx::setViewClear(mViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, mClearColor.ToRGBA(), 1.0f, 0);
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		bgfx::setViewTransform(mViewId, GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
	else
	{
		bgfx::setViewTransform(mViewId, GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
	const Vector2u framebufferSize = BgfxWrapper::GetFramebufferSize(mFramebuffer);
	const Vector2f vpMin = mViewport.GetMin();
	const Vector2f vpSize = mViewport.GetSize();
 	bgfx::setViewRect(mViewId, static_cast<U16>(vpMin.x * framebufferSize.x), static_cast<U16>(vpMin.y * framebufferSize.y), static_cast<U16>(vpSize.x * framebufferSize.x), static_cast<U16>(vpSize.y * framebufferSize.y));
	bgfx::setViewFrameBuffer(mViewId, mFramebuffer);
	bgfx::touch(mViewId);
}

Frustum Camera::CreateFrustum() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return Frustum(perspective.fov, perspective.aspect, perspective.nearPlane, perspective.farPlane, mPosition, mPosition + mRotation.TransformDirection(ENLIVE_DEFAULT_FORWARD), ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	}
	else
	{
		return Frustum(orthographic.left, orthographic.top, orthographic.right, orthographic.bottom, orthographic.nearPlane, orthographic.farPlane, mPosition, mPosition + mRotation.TransformDirection(ENLIVE_DEFAULT_FORWARD), ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	}
}

void Camera::SetProjection(ProjectionMode projection)
{
	mProjectionMode = projection;
	mProjectionDirty = true;
}

Camera::ProjectionMode Camera::GetProjection() const
{
	return mProjectionMode;
}

void Camera::InitializePerspective(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane)
{
	mProjectionMode = ProjectionMode::Perspective;
	perspective.nearPlane = nearPlane;
	perspective.farPlane = farPlane;
	perspective.fov = fov;
	perspective.aspect = aspect;
	mProjectionDirty = true;
}

void Camera::InitializeOrthographic(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane)
{
	mProjectionMode = ProjectionMode::Orthographic;
	orthographic.nearPlane = nearPlane;
	orthographic.farPlane = farPlane;
	orthographic.left = left;
	orthographic.top = top;
	orthographic.right = right;
	orthographic.bottom = bottom;
	mProjectionDirty = true;
}

void Camera::SetNearPlane(F32 nearPlane)
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		perspective.nearPlane = nearPlane;
	}
	else
	{
		orthographic.nearPlane = nearPlane;
	}
	mProjectionDirty = true;
}

F32 Camera::GetNearPlane() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return perspective.nearPlane;
	}
	else
	{
		return orthographic.nearPlane;
	}
}

void Camera::SetFarPlane(F32 farPlane)
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		perspective.farPlane = farPlane;
	}
	else
	{
		orthographic.farPlane = farPlane;
	}
	mProjectionDirty = true;
}

F32 Camera::GetFarPlane() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return perspective.farPlane;
	}
	else
	{
		return orthographic.farPlane;
	}
}

void Camera::SetFOV(F32 fov)
{
	enAssert(mProjectionMode == ProjectionMode::Perspective);
	perspective.fov = fov;
	mProjectionDirty = true;
}

F32 Camera::GetFOV() const
{
	enAssert(mProjectionMode == ProjectionMode::Perspective);
	return perspective.fov;
}

void Camera::SetAspect(F32 aspect)
{
	enAssert(mProjectionMode == ProjectionMode::Perspective);
	perspective.aspect = aspect;
	mProjectionDirty = true;
}

F32 Camera::GetAspect() const
{
	enAssert(mProjectionMode == ProjectionMode::Perspective);
	return perspective.aspect;
}

void Camera::SetLeft(F32 left)
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.left = left;
	mProjectionDirty = true;
}

F32 Camera::GetLeft() const
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.left;
}

void Camera::SetTop(F32 top)
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.top = top;
	mProjectionDirty = true;
}

F32 Camera::GetTop() const
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.top;
}

void Camera::SetRight(F32 right)
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.right = right;
	mProjectionDirty = true;
}

F32 Camera::GetRight() const
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.right;
}

void Camera::SetBottom(F32 bottom)
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.bottom = bottom;
	mProjectionDirty = true;
}

F32 Camera::GetBottom() const
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.bottom;
}

const Matrix4f& Camera::GetProjectionMatrix() const
{
	if (mProjectionDirty)
	{
		UpdateProjectionMatrix();
	}
	return mProjectionMatrix;
}

void Camera::InitializeView(const Vector3f& position, const Matrix3f& rotation)
{
	mPosition = position;
	mRotation = rotation;
	mViewDirty = true;
}

void Camera::SetPosition(const Vector3f& position)
{
	mPosition = position;
	mViewDirty = true;
}

const Vector3f& Camera::GetPosition() const
{
	return mPosition;
}

void Camera::Move(const Vector3f& movement)
{
	mPosition += movement;
	mViewDirty = true;
}

void Camera::SetRotation(const Matrix3f& rotation)
{
	mRotation = rotation;
	mViewDirty = true;
}

const Matrix3f& Camera::GetRotation() const
{
	return mRotation;
}

void Camera::Rotate(const Matrix3f& rotation)
{
	mRotation *= rotation;
	mViewDirty = true;
}

const Matrix4f& Camera::GetViewMatrix() const
{
	if (mViewDirty)
	{
		UpdateViewMatrix();
	}
	return mViewMatrix;
}

void Camera::SetClearColor(const Color& clearColor)
{
	mClearColor = clearColor;
}

const Color& Camera::GetClearColor() const
{
	return mClearColor;
}

void Camera::SetViewport(const Rectf& viewport)
{
	mViewport = viewport;
}

const en::Rectf& Camera::GetViewport() const
{
	return mViewport;
}

void Camera::SetFramebuffer(bgfx::FrameBufferHandle framebuffer)
{
	mFramebuffer = framebuffer;
}

bgfx::FrameBufferHandle Camera::GetFramebuffer() const
{
	return mFramebuffer;
}

bgfx::ViewId Camera::GetViewID() const
{
	return mViewId;
}

void Camera::UpdateProjectionMatrix() const
{
	const bool homogenousDepth = bgfx::getCaps()->homogeneousDepth;
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		mProjectionMatrix = Matrix4f::Perspective(perspective.fov, perspective.aspect, perspective.nearPlane, perspective.farPlane, homogenousDepth, ENLIVE_DEFAULT_HANDEDNESS);
	}
	else
	{
		mProjectionMatrix = Matrix4f::Orthographic(orthographic.left, orthographic.right, orthographic.bottom, orthographic.top, orthographic.nearPlane, orthographic.farPlane, homogenousDepth, ENLIVE_DEFAULT_HANDEDNESS);
	}
	mProjectionDirty = false;
}

void Camera::UpdateViewMatrix() const
{
	mViewMatrix = Matrix4f::LookAt(mPosition, mPosition + mRotation.GetForward(), ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	mViewDirty = false;
}

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS