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
	~Camera();

	Camera(Camera&& other) noexcept;
	Camera(const Camera& other) = delete;

	Camera& operator=(Camera&& other) noexcept;
	Camera& operator=(const Camera& other) = delete;

	virtual void Apply() const;

	virtual Frustum CreateFrustum() const;

	virtual Vector3f ScreenToWorldPoint(const Vector2i& screenCoordinates, Vector3f* outDirection = nullptr) const;

    // Projection

    enum class ProjectionMode
    {
        Perspective,
        Orthographic
    };
    void SetProjection(ProjectionMode projection);
    ProjectionMode GetProjection() const;

    void InitializePerspective(F32 fov, F32 nearPlane = 0.1f, F32 farPlane = 100.0f);
	void InitializeOrthographic(F32 size, F32 nearPlane = 0.1f, F32 farPlane = 100.0f);

	void SetNearPlane(F32 nearPlane);
	F32 GetNearPlane() const;

	void SetFarPlane(F32 farPlane);
	F32 GetFarPlane() const;

	void SetFOV(F32 fov);
	F32 GetFOV() const;

	void SetSize(F32 size);
	F32 GetSize() const;

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

	void SetFramebuffer(Framebuffer* framebuffer);
	Framebuffer* GetFramebuffer() const;

	bgfx::ViewId GetViewID() const;

	// Camera
	static void SetMainCamera(Camera* camera);
	static Camera* GetMainCamera();

protected:
	void UpdateProjectionMatrix() const;
	void UpdateViewMatrix() const;
	F32 GetAspect() const;

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
	Framebuffer* mFramebuffer;
	enSlotType(Framebuffer, OnResized) mFramebufferResized;
	bgfx::ViewId mViewId;
	ProjectionMode mProjectionMode;
	mutable bool mProjectionDirty;
	mutable bool mViewDirty;

	static bgfx::ViewId sViewIdCounter;

	static constexpr U32 kMaxCameras = 10;
	static U32 sCameraCount;
	static Camera* sCameras[kMaxCameras];
	static void RegisterCamera(Camera* camera);
	static void UnregisterCamera(Camera* camera);
	static Camera* sMainCamera;
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO(en::Camera::ProjectionMode)

ENLIVE_DEFINE_TYPE_INFO(en::Camera)