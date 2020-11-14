#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Math/Transform.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

class TransformComponent : public Transform
{
    public:
		TransformComponent();
		~TransformComponent();

		TransformComponent(TransformComponent&& other) noexcept;
		TransformComponent(const TransformComponent&) = delete;

		TransformComponent& operator=(TransformComponent&& other) noexcept;
		TransformComponent& operator=(const TransformComponent&) = delete;

		Entity GetEntity() const;
		World* GetWorld();
		const World* GetWorld() const;

	private:
		friend struct ComponentHasCustomInitialization<TransformComponent>;

		Entity mEntity;
};

} // namespace en

template <>
struct ComponentHasCustomInitialization<en::TransformComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, en::TransformComponent& component)
	{
		if (entity.IsValid())
		{
			component.mEntity = entity;
		}
		return false;
	}
};

#endif // ENLIVE_MODULE_CORE