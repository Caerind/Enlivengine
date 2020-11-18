#pragma once

#include <bgfx/bgfx.h>

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Rect.hpp>

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

    // Projection

    enum class ProjectionMode
    {
        Perspective,
        Orthographic
    };
    void SetProjection(ProjectionMode projection);
    ProjectionMode GetProjection() const;

    void InitializePerspective(F32 fov, F32 aspect, F32 nearPlane, F32 farPlane);
	void InitializeOrthographic(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane);

	void SetNearPlane(F32 nearPlane);
	F32 GetNearPlane() const;

	void SetFarPlane(F32 farPlane);
	F32 GetFarPlane() const;

	void SetFOV(F32 fov);
	F32 GetFOV() const;

	void SetAspect(F32 aspect);
	F32 GetAspect() const;

	void SetLeft(F32 left);
	F32 GetLeft() const;

	void SetTop(F32 top);
	F32 GetTop() const;

	void SetRight(F32 right);
	F32 GetRight() const;

	void SetBottom(F32 bottom); 
	F32 GetBottom() const;

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

	void SetFramebuffer(bgfx::FrameBufferHandle framebuffer);
	bgfx::FrameBufferHandle GetFramebuffer() const;

	bgfx::ViewId GetViewID() const;

protected:
	void UpdateProjectionMatrix() const;
	void UpdateViewMatrix() const;

    struct PerspectiveData
	{
		F32 nearPlane;
		F32 farPlane;
		F32 fov;
		F32 aspect;
	};

	struct OrthographicData
	{
		F32 nearPlane;
		F32 farPlane;
		F32 left;
		F32 top;
		F32 right;
		F32 bottom;
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
	bgfx::FrameBufferHandle mFramebuffer;
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
};

} // namespace en