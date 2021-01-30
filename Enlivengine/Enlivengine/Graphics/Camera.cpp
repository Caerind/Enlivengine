#include <Enlivengine/Graphics/Camera.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Graphics/BgfxWrapper.hpp>

namespace en
{

bgfx::ViewId Camera::sViewIdCounter = 0;
U32 Camera::sCameraCount = 0;
Camera* Camera::sCameras[kMaxCameras];
Camera* Camera::sMainCamera = nullptr;

Camera::Camera()
	: mViewMatrix()
	, mProjectionMatrix()
	, mRotation()
	, mViewport(Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, mPosition()
	, mProjectionData()
	, mClearColor(U32(0x443355FF))
	, mAspect(4.0f/3.0f)
	, mProjectionMode(ProjectionMode::Perspective)
	, mProjectionDirty(true)
	, mViewDirty(true)
	, mMainCamera(false)
{
	RegisterCamera(this);
	InitializePerspective();
}

Camera::~Camera()
{
	UnregisterCamera(this);
	if (sMainCamera == this)
	{
		sMainCamera = nullptr;
	}
}

Camera::Camera(Camera&& other) noexcept
	: mViewMatrix(other.mViewMatrix)
	, mProjectionMatrix(other.mProjectionMatrix)
	, mRotation(other.mRotation)
	, mViewport(other.mViewport)
	, mPosition(other.mPosition)
	, mProjectionData(other.mProjectionData)
	, mClearColor(other.mClearColor)
	, mAspect(other.mAspect)
	, mProjectionMode(other.mProjectionMode)
	, mProjectionDirty(other.mProjectionDirty)
	, mViewDirty(other.mViewDirty)
{
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	if (&other != this)
	{
		mViewMatrix = other.mViewMatrix;
		mProjectionMatrix = other.mProjectionMatrix;
		mRotation = other.mRotation;
		mViewport = other.mViewport;
		mPosition = other.mPosition;
		mProjectionData = other.mProjectionData;
		mClearColor = other.mClearColor;
		mAspect = other.mAspect;
		mProjectionMode = other.mProjectionMode;
		mProjectionDirty = other.mProjectionDirty;
		mViewDirty = other.mViewDirty;
	}
	return *this;
}

Frustum Camera::CreateFrustum() const
{
	const F32 aspect = GetAspect();
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return Frustum(mProjectionData.perspective.fov, aspect, mProjectionData.perspective.nearPlane, mProjectionData.perspective.farPlane, mPosition, mPosition + mRotation.TransformDirection(ENLIVE_DEFAULT_FORWARD), ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	}
	else
	{
		return Frustum(-mProjectionData.orthographic.size * aspect, -mProjectionData.orthographic.size, mProjectionData.orthographic.size * aspect, mProjectionData.orthographic.size, mProjectionData.orthographic.nearPlane, mProjectionData.orthographic.farPlane, mPosition, mPosition + mRotation.TransformDirection(ENLIVE_DEFAULT_FORWARD), ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	}
}

Vector3f Camera::ScreenToWorldPoint(const Vector2u& fbSize, const Vector2i& screenCoordinates, Vector3f* outDirection) const
{
	enAssert(fbSize.x > 0);
	enAssert(fbSize.y > 0);

	const Frustum frustum = CreateFrustum();

	const F32 ratioX = static_cast<F32>(screenCoordinates.x) / fbSize.x;
	const F32 ratioY = static_cast<F32>(screenCoordinates.y) / fbSize.y;
	const Vector3f& nearOrigin = frustum.GetCorner(Frustum::Corners::NTL);
	const Vector3f nearUnitX = (frustum.GetCorner(Frustum::Corners::NTR) - nearOrigin);
	const Vector3f nearUnitY = (frustum.GetCorner(Frustum::Corners::NBL) - nearOrigin);
	const Vector3f nearResult = nearOrigin + nearUnitX * ratioX + nearUnitY * ratioY;

	if (outDirection != nullptr)
	{
		const Vector3f& farOrigin = frustum.GetCorner(Frustum::Corners::FTL);
		const Vector3f farUnitX = (frustum.GetCorner(Frustum::Corners::FTR) - farOrigin);
		const Vector3f farUnitY = (frustum.GetCorner(Frustum::Corners::FBL) - farOrigin);
		const Vector3f farResult = farOrigin + farUnitX * ratioX + farUnitY * ratioY;
		*outDirection = (farResult - nearResult).Normalized();
	}

	return nearResult;
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

void Camera::InitializePerspective(F32 fov, F32 nearPlane, F32 farPlane)
{
	mProjectionMode = ProjectionMode::Perspective;
	mProjectionData.perspective.nearPlane = nearPlane;
	mProjectionData.perspective.farPlane = farPlane;
	mProjectionData.perspective.fov = fov;
	mProjectionDirty = true;
}

void Camera::InitializeOrthographic(F32 size, F32 nearPlane, F32 farPlane)
{
	mProjectionMode = ProjectionMode::Orthographic;
	mProjectionData.orthographic.nearPlane = nearPlane;
	mProjectionData.orthographic.farPlane = farPlane;
	mProjectionData.orthographic.size = size;
	mProjectionDirty = true;
}

void Camera::SetNearPlane(F32 nearPlane)
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		mProjectionData.perspective.nearPlane = nearPlane;
	}
	else
	{
		mProjectionData.orthographic.nearPlane = nearPlane;
	}
	mProjectionDirty = true;
}

F32 Camera::GetNearPlane() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return mProjectionData.perspective.nearPlane;
	}
	else
	{
		return mProjectionData.orthographic.nearPlane;
	}
}

void Camera::SetFarPlane(F32 farPlane)
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		mProjectionData.perspective.farPlane = farPlane;
	}
	else
	{
		mProjectionData.orthographic.farPlane = farPlane;
	}
	mProjectionDirty = true;
}

F32 Camera::GetFarPlane() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return mProjectionData.perspective.farPlane;
	}
	else
	{
		return mProjectionData.orthographic.farPlane;
	}
}

void Camera::SetFOV(F32 fov)
{
	enAssert(mProjectionMode == ProjectionMode::Perspective);
	mProjectionData.perspective.fov = fov;
	mProjectionDirty = true;
}

F32 Camera::GetFOV() const
{
	enAssert(mProjectionMode == ProjectionMode::Perspective);
	return mProjectionData.perspective.fov;
}

void Camera::SetSize(F32 size)
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	mProjectionData.orthographic.size = size;
	mProjectionDirty = true;
}

F32 Camera::GetSize() const
{
	enAssert(mProjectionMode == ProjectionMode::Orthographic);
	return mProjectionData.orthographic.size;
}

void Camera::SetAspect(F32 aspect)
{
	mAspect = aspect;
	mProjectionDirty = true;
}

F32 Camera::GetAspect() const
{
	return mAspect;
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

const Rectf& Camera::GetViewport() const
{
	return mViewport;
}

void Camera::SetMainCamera(bool mainCamera)
{
	if (!mainCamera)
	{
		if (IsCurrentMainCamera())
		{
			mMainCamera = false;
			sMainCamera = nullptr;
			for (U32 i = 0; i < sCameraCount; ++i)
			{
				if (sCameras[i] != nullptr && sCameras[i]->IsMainCamera())
				{
					sMainCamera = sCameras[i];
				}
			}
		}
		else
		{
			mMainCamera = false;
		}
	}
	else
	{
		// TODO : Priority to the current sMainCamera or the newest ?
		if (sMainCamera == nullptr)
		{
			sMainCamera = this;
		}
		mMainCamera = true;
	}
}

bool Camera::IsMainCamera() const
{
	return mMainCamera;
}

bool Camera::IsCurrentMainCamera() const
{
	return mMainCamera && sMainCamera == this;
}

Camera* Camera::GetMainCamera()
{
	return sMainCamera;
}

bool Camera::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Camera>::GetName(), TypeInfo<Camera>::GetHash()))
	{
		bool ret = true;
		ret = GenericSerialization(serializer, "rotation", mRotation) && ret;
		ret = GenericSerialization(serializer, "position", mPosition) && ret;
		ret = GenericSerialization(serializer, "viewport", mViewport) && ret;
		ret = GenericSerialization(serializer, "clearColor", mClearColor) && ret;
		ret = GenericSerialization(serializer, "projectionMode", mProjectionMode) && ret;
		//ret = GenericSerialization(serializer, "aspect", mAspect) && ret;
		if (mProjectionMode == ProjectionMode::Perspective)
		{
			ret = GenericSerialization(serializer, "nearPlane", mProjectionData.perspective.nearPlane) && ret;
			ret = GenericSerialization(serializer, "farPlane", mProjectionData.perspective.farPlane) && ret;
			ret = GenericSerialization(serializer, "fov", mProjectionData.perspective.fov) && ret;
		}
		else
		{
			ret = GenericSerialization(serializer, "nearPlane", mProjectionData.orthographic.nearPlane) && ret;
			ret = GenericSerialization(serializer, "farPlane", mProjectionData.orthographic.farPlane) && ret;
			ret = GenericSerialization(serializer, "size", mProjectionData.orthographic.size) && ret;
		}

		if (serializer.IsReading())
		{
			bool mainCamera = false;
			if (GenericSerialization(serializer, "mainCamera", mainCamera))
			{
				SetMainCamera(mainCamera);
			}
			else
			{
				ret = false;
			}

			mViewDirty = true;
			mProjectionDirty = true;
		}
		else if (serializer.IsWriting())
		{
			ret = GenericSerialization(serializer, "mainCamera", mMainCamera) && ret;
		}
		else
		{
			enAssert(false);
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool Camera::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<Camera>::GetName(), TypeInfo<Camera>::GetHash()))
	{
		bool ret = false;
		ret = GenericEdit(objectEditor, "Projection", mProjectionMode) || ret;
		if (mProjectionMode == ProjectionMode::Perspective)
		{
			ret = GenericEdit(objectEditor, "NearPlane", mProjectionData.perspective.nearPlane) || ret;
			ret = GenericEdit(objectEditor, "FarPlane", mProjectionData.perspective.farPlane) || ret;
			ret = GenericEdit(objectEditor, "FOV", mProjectionData.perspective.fov) || ret;
		}
		else
		{
			ret = GenericEdit(objectEditor, "NearPlane", mProjectionData.orthographic.nearPlane) || ret;
			ret = GenericEdit(objectEditor, "FarPlane", mProjectionData.orthographic.farPlane) || ret;
			ret = GenericEdit(objectEditor, "Size", mProjectionData.orthographic.size) || ret;
		}

		ret = GenericEdit(objectEditor, "ClearColor", mClearColor) || ret;
		ret = GenericEdit(objectEditor, "Viewport", mViewport) || ret;
		//ret = GenericEdit(objectEditor, "Aspect", mAspect) || ret;

		bool mainCamera = mMainCamera;
		if (GenericEdit(objectEditor, "MainCamera", mainCamera))
		{
			SetMainCamera(mainCamera);
			enAssert(mainCamera == mMainCamera);
			ret = true;
		}

		if (ret)
		{
			mViewDirty = true;
			mProjectionDirty = true;
		}

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

void Camera::UpdateProjectionMatrix() const
{
	const bool homogenousDepth = bgfx::getCaps()->homogeneousDepth;
	const F32 aspect = GetAspect();
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		mProjectionMatrix = Matrix4f::Perspective(mProjectionData.perspective.fov, aspect, mProjectionData.perspective.nearPlane, mProjectionData.perspective.farPlane, homogenousDepth, ENLIVE_DEFAULT_HANDEDNESS);
	}
	else
	{
		mProjectionMatrix = Matrix4f::Orthographic(-mProjectionData.orthographic.size * aspect, -mProjectionData.orthographic.size, mProjectionData.orthographic.size * aspect, mProjectionData.orthographic.size, mProjectionData.orthographic.nearPlane, mProjectionData.orthographic.farPlane, homogenousDepth, ENLIVE_DEFAULT_HANDEDNESS);
	}
	mProjectionDirty = false;
}

void Camera::UpdateViewMatrix() const
{
	mViewMatrix = Matrix4f::LookAt(mPosition, mPosition + mRotation.GetForward(), ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	mViewDirty = false;
}

void Camera::RegisterCamera(Camera* camera)
{
	static bool initialized = false;
	if (!initialized)
	{
		for (U32 i = 0; i < kMaxCameras; ++i)
		{
			sCameras[i] = nullptr;
		}
		initialized = true;
	}

	if (sCameraCount < kMaxCameras)
	{
		sCameras[sCameraCount] = camera;
		sCameraCount++;
	}
	else
	{
		// Maximum amount of cameras reached
		// Increase Camera::kMaxCameras or use less cameras
		enAssert(false);
	}
}

void Camera::UnregisterCamera(Camera* camera)
{
	for (U32 i = 0; i < sCameraCount; ++i)
	{
		if (sCameras[i] == camera)
		{
			sCameraCount--;
			for (U32 j = i; j < sCameraCount; ++j)
			{
				sCameras[j] = sCameras[j + 1];
			}
			return;
		}
	}
	// Not found ?
	enAssert(false);
}

} // namespace en