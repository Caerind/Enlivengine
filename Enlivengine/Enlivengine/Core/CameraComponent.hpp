#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Graphics/Camera.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

class CameraComponent
{
    public:
        CameraComponent();
        CameraComponent(CameraComponent&& other) noexcept;
        CameraComponent(const CameraComponent&) = delete;
		~CameraComponent();

        CameraComponent& operator=(CameraComponent&& other) noexcept;
		CameraComponent& operator=(const CameraComponent&) = delete;

		void Apply() const;

		Frustum CreateFrustum() const;

		// Projection

		void SetProjection(Camera::ProjectionMode projection);
		Camera::ProjectionMode GetProjection() const;

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
		// TODO : Remove from CameraComponent (but later, as it is handy for now)

		void InitializeView(const Vector3f& position, const Matrix3f& rotation);

		void SetPosition(const Vector3f& position);
		const Vector3f& GetPosition() const;
		void Move(const Vector3f& movement);

		void SetRotation(const Matrix3f& rotation);
		const Matrix3f& GetRotation() const;
		void Rotate(const Matrix3f& rotation);

		const Matrix4f& GetViewMatrix() const;

		// View Options

		void SetClearColor(const Color& clearColor);
		const Color& GetClearColor() const;

		void SetViewport(const Rectf& viewport);
		const Rectf& GetViewport() const;

		void SetFramebuffer(bgfx::FrameBufferHandle framebuffer);
		bgfx::FrameBufferHandle GetFramebuffer() const;

		bgfx::ViewId GetViewID() const;

		// Entity & World

		Entity GetEntity() const;
		World* GetWorld();
		const World* GetWorld() const;

	private:
		const Matrix4f& GetEntityMatrix() const;

	private:
		template <typename T> friend struct ComponentHasCustomInitialization;
		template <typename T> friend struct HasCustomEditor;

		Camera mCamera;
		mutable Matrix4f mViewMatrix;
		Entity mEntity;
};

} // namespace en

template <>
struct ComponentHasCustomInitialization<en::CameraComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, en::CameraComponent& component)
	{
		if (entity.IsValid())
		{
			component.mEntity = entity;
		}
		return false;
	}
};

#endif // ENLIVE_MODULE_CORE