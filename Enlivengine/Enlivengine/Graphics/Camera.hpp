#pragma once

#include <bgfx/bgfx.h>

#include <Enlivengine/Utils/Signal.hpp>

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Rect.hpp>

#include <Enlivengine/Graphics/Framebuffer.hpp>

namespace en
{

class Camera
{
public:
	Camera();
	virtual ~Camera();

	Camera(Camera&& other) noexcept;
	Camera(const Camera& other) = delete;

	Camera& operator=(Camera&& other) noexcept;
	Camera& operator=(const Camera& other) = delete;

	virtual Frustum CreateFrustum() const;

	virtual Vector3f ScreenToWorldPoint(const Vector2u& fbSize, const Vector2i& screenCoordinates, Vector3f* outDirection = nullptr) const;

    // Projection

    enum class ProjectionMode
    {
        Perspective,
        Orthographic
    };
    void SetProjection(ProjectionMode projection);
    ProjectionMode GetProjection() const;

    void InitializePerspective(F32 fov = 80.0f, F32 nearPlane = 0.1f, F32 farPlane = 300.0f);
	void InitializeOrthographic(F32 size, F32 nearPlane = 0.1f, F32 farPlane = 300.0f);

	void SetNearPlane(F32 nearPlane);
	F32 GetNearPlane() const;

	void SetFarPlane(F32 farPlane);
	F32 GetFarPlane() const;

	void SetFOV(F32 fov);
	F32 GetFOV() const;

	void SetSize(F32 size);
	F32 GetSize() const;

	void SetAspect(F32 aspect);
	F32 GetAspect() const;

    const Matrix4f& GetProjectionMatrix() const;

    // View

	void InitializeView(const Vector3f& position, const Matrix3f& rotation);

	void SetPosition(const Vector3f& position);
	const Vector3f& GetPosition() const;
	void Move(const Vector3f& movement);

	void SetRotation(const Matrix3f& rotation);
	const Matrix3f& GetRotation() const;
	void Rotate(const Matrix3f& rotation);

    virtual const Matrix4f& GetViewMatrix() const;

	// View Options

	void SetClearColor(const Color& clearColor);
	const Color& GetClearColor() const;

	void SetViewport(const Rectf& viewport);
	const Rectf& GetViewport() const;

	// Camera

	void SetMainCamera(bool mainCamera);
	bool IsMainCamera() const;
	bool IsCurrentMainCamera() const;
	static Camera* GetMainCamera();

	// Common

	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

protected:
	void UpdateProjectionMatrix() const;
	void UpdateViewMatrix() const;

    struct PerspectiveData
	{
		F32 nearPlane;
		F32 farPlane;
		F32 fov;
	};

	struct OrthographicData
	{
		F32 nearPlane;
		F32 farPlane;
		F32 size;
	};

	mutable Matrix4f mViewMatrix;
    mutable Matrix4f mProjectionMatrix;
	Matrix3f mRotation;
	Rectf mViewport;
	Vector3f mPosition;
	union
	{
		PerspectiveData perspective;
		OrthographicData orthographic;
	} mProjectionData;
	Color mClearColor;
	F32 mAspect;
	ProjectionMode mProjectionMode;
	mutable bool mProjectionDirty;
	mutable bool mViewDirty;
	bool mMainCamera;

	static bgfx::ViewId sViewIdCounter;

	static constexpr U32 kMaxCameras = 10;
	static U32 sCameraCount;
	static Camera* sCameras[kMaxCameras];
	static void RegisterCamera(Camera* camera);
	static void UnregisterCamera(Camera* camera);
	static Camera* sMainCamera;
};

} // namespace en

ENLIVE_DEFINE_ENUM_INFO(en::Camera::ProjectionMode)

ENLIVE_DEFINE_TYPE_INFO(en::Camera, en::Type_CustomSerialization, en::Type_CustomEditor)