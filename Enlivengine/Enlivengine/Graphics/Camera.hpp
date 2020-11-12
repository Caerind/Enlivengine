#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <bgfx/bgfx.h>

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Frustum.hpp>

namespace en
{

class Camera
{
public:
	Camera();

	void Apply(bgfx::ViewId viewId) const;

	Frustum CreateFrustum() const;

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

	void InitializeView(const Vector3f& position, const Quaternionf& rotation);

	void SetPosition(const Vector3f& position);
	const Vector3f& GetPosition() const;
	void Move(const Vector3f& movement);

	void SetRotation(const Quaternionf& rotation);
	const Quaternionf& GetRotation() const;
	void Rotate(const Quaternionf& rotation);

    const Matrix4f& GetViewMatrix() const;

private:
	void UpdateProjectionMatrix() const;
	void UpdateViewMatrix() const;

private:
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
	Quaternionf mRotation;
	Vector3f mPosition;
	union
	{
		PerspectiveData perspective;
		OrthographicData orthographic;
	};
	ProjectionMode mProjectionMode;
	mutable bool mProjectionDirty;
	mutable bool mViewDirty;
};

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS