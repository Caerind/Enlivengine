#pragma once

#include <vector>

#include <Enlivengine/Math/Transform.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/EntityHandle.hpp>

// TODO : Ensure children/parent/entity are from the same world
// TODO : What if children become invalid or loose its transform component

namespace en
{

class TransformComponent
{
public:
	TransformComponent();
	~TransformComponent();

	TransformComponent(TransformComponent&& other) noexcept;
	TransformComponent(const TransformComponent&) = delete;

	TransformComponent& operator=(TransformComponent&& other) noexcept;
	TransformComponent& operator=(const TransformComponent&) = delete;

	// Position
	Vector3f GetPosition() const;
	void SetPosition(const Vector3f& position);
	void Move(const Vector3f& movement);

	// Rotation
	Matrix3f GetRotation() const;
	void SetRotation(const Matrix3f& rotation);
	void Rotate(const Matrix3f& rotation);

	// Scale
	Vector3f GetScale() const;
	void SetScale(const Vector3f& scale);
	void SetUniformScale(F32 uniformScale);
	void Scale(const Vector3f& scale);
	void Scale(F32 uniformScale);

	// Transform
	void SetTransform(const Vector3f& translation, const Matrix3f& rotation, const Vector3f& scale = Vector3f(1.0f));

	// Children
	bool CanAttach(const Entity& childEntity) const;
	void AttachChild(const Entity& childEntity);
	void DetachChild(const Entity& childEntity);
	bool IsChild(const Entity& childEntity) const;
	bool IsChildOrSubchild(const Entity& childEntity) const;
	U32 GetChildrenCount() const;
	const EntityHandle& GetChild(U32 index) const;

	// Parent
	void AttachToParent(const Entity& parentEntity);
	void DetachFromParent();
	bool HasParent() const;
	const EntityHandle& GetParent() const;

	// Global
	const Matrix4f& GetGlobalMatrix() const;
	Vector3f GetGlobalPosition() const;
	Matrix3f GetGlobalRotation() const;

	// Local
	const Matrix4f& GetLocalMatrix() const;

	// TODO : Should hide this, but now it it used by ImGuizmo
	void MarkGlobalMatrixAsDirty();

	bool Initialize(const Entity& entity);
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

	// Entity
	const Entity& GetEntity() const;

private:
	void UpdateGlobalMatrix() const;

private:
	friend struct ComponentHasCustomInitialization<TransformComponent>;
	Transform mLocalTransform;
	mutable Matrix4f mGlobalMatrix;
	Entity mEntity;
	EntityHandle mParent;
	std::vector<EntityHandle> mChildren;
	mutable bool mGlobalMatrixDirty;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::TransformComponent, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()
ENLIVE_COMPONENT_CUSTOM_INITIALIZATION(en::TransformComponent)