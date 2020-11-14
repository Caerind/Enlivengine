#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Graphics/Camera.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

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

		Entity GetEntity() const;
		World* GetWorld();
		const World* GetWorld() const;

	private:
		const Matrix4f& GetEntityMatrix() const;

	private:
		template <typename T> friend struct ComponentHasCustomInitialization;
		template <typename T> friend struct HasCustomEditor;

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