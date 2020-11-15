#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <vector>

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

	void AttachChild(const Entity& childEntity);
	void DetachChild(const Entity& childEntity);
	bool HasChild(const Entity& childEntity) const;
	U32 GetChildrenCount() const;
	Entity GetChild(U32 index) const;

	void AttachToParent(const Entity& parentEntity);
	void DetachFromParent();
	bool HasParent() const;
	Entity GetParent() const;

	Matrix4f GetGlobalMatrix() const;
	Vector3f GetGlobalPosition() const;

	Matrix4f& GetLocalMatrix();
	const Matrix4f& GetLocalMatrix() const;

	Entity GetEntity() const;
	World* GetWorld();
	const World* GetWorld() const;

private:
	friend struct ComponentHasCustomInitialization<TransformComponent>;

	Entity mEntity;
	Entity mParent;
	std::vector<Entity> mChildren;
};

} // namespace en

template <>
struct ComponentHasCustomInitialization<en::TransformComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, en::TransformComponent& component)
	{
		enAssert(entity.IsValid());
		component.mEntity = entity;
		return true;
	}
};

#endif // ENLIVE_MODULE_CORE