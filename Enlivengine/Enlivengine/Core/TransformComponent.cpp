#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Core/Components.hpp>

namespace en
{

TransformComponent::TransformComponent()
	: Transform()
	, mGlobalMatrix(Matrix4f::Identity())
	, mEntity()
	, mParent()
	, mChildren()
	, mGlobalMatrixDirty(true)
{
}

TransformComponent::~TransformComponent()
{
	DetachFromParent();
}

TransformComponent::TransformComponent(TransformComponent&& other) noexcept
	: Transform(other)
	, mGlobalMatrix(other.mGlobalMatrix)
	, mEntity(other.mEntity)
	, mParent(other.mParent)
	, mChildren(std::move(other.mChildren))
	, mGlobalMatrixDirty(other.mGlobalMatrixDirty)
{
	other.mEntity = Entity();
	other.mParent = Entity();
	other.mChildren.clear();
}

TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
{
	if (&other != this)
	{
		Transform::operator=(other);
		mGlobalMatrix = other.mGlobalMatrix;
		mEntity = other.mEntity;
		mParent = other.mParent;
		mChildren = std::move(other.mChildren);
		mGlobalMatrixDirty = other.mGlobalMatrixDirty;
		other.mEntity = Entity();
		other.mParent = Entity();
		other.mChildren.clear();
	}
	return *this;
}

void TransformComponent::SetPosition(const Vector3f& position)
{
	Transform::SetPosition(position);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Move(const Vector3f& movement)
{
	Transform::Move(movement);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::SetRotation(const Matrix3f& rotation)
{
	Transform::SetRotation(rotation);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Rotate(const Matrix3f& rotation)
{
	Transform::Rotate(rotation);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::SetScale(const Vector3f& scale)
{
	Transform::SetScale(scale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::SetUniformScale(F32 uniformScale)
{
	Transform::SetUniformScale(uniformScale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Scale(const Vector3f& scale)
{
	Transform::Scale(scale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Scale(F32 uniformScale)
{
	Transform::Scale(uniformScale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::SetTransform(const Vector3f& translation, const Matrix3f& rotation, const Vector3f& scale /*= Vector3f(1.0f)*/)
{
	Transform::SetTransform(translation, rotation, scale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::AttachChild(const Entity& childEntity)
{
	enAssert(childEntity != mEntity);
	enAssert(mEntity.IsValid());
	enAssert(childEntity.IsValid());
	enAssert(childEntity.Has<TransformComponent>());
	TransformComponent& childTransform = const_cast<TransformComponent&>(childEntity.Get<TransformComponent>());

	// Skip if already the parent
	if (childTransform.mParent == mEntity)
	{
		return;
	}

	// Detach the current parent of the child
	if (childTransform.mParent.IsValid())
	{
		enAssert(childTransform.mParent.Has<TransformComponent>());
		enAssert(childTransform.mParent.Get<TransformComponent>().HasChild(childEntity));
		childTransform.mParent.Get<TransformComponent>().DetachChild(childEntity);
	}

	// Set the new parent & matrix
	const Matrix4f result = childTransform.GetGlobalMatrix() * GetGlobalMatrix().Inversed();
	childTransform.mParent = mEntity;
	childTransform.SetTransform(result.GetTranslation(), result.GetRotation(), result.GetScale());

	// Store the child on parent's side
	mChildren.push_back(childEntity);
}

void TransformComponent::DetachChild(const Entity& childEntity)
{
	enAssert(mEntity.IsValid());
	enAssert(childEntity.IsValid());
	enAssert(childEntity.Has<TransformComponent>());
	TransformComponent& childTransform = const_cast<TransformComponent&>(childEntity.Get<TransformComponent>());

	// It should be the parent
	enAssert(childTransform.mParent == mEntity);

	// Reset the parent & matrix
	const Matrix4f result = childTransform.GetGlobalMatrix();
	childTransform.mParent = Entity();
	childTransform.SetTransform(result.GetTranslation(), result.GetRotation(), result.GetScale());

	// Remove the child on parent's side
	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		if (mChildren[i] == childEntity)
		{
			mChildren.erase(mChildren.begin() + i);
			return;
		}
	}
	enAssert(false); // Was not a stored child on parent's side
}

bool TransformComponent::HasChild(const Entity& childEntity) const
{
	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		if (mChildren[i] == childEntity)
		{
			return true;
		}
	}
	return false;
}

U32 TransformComponent::GetChildrenCount() const
{
	return static_cast<U32>(mChildren.size());
}

Entity TransformComponent::GetChild(U32 index) const
{
	enAssert(index < GetChildrenCount());
	return mChildren[index];
}

void TransformComponent::AttachToParent(const Entity& parentEntity)
{
	enAssert(mEntity.IsValid());
	enAssert(parentEntity.IsValid());
	enAssert(parentEntity.Has<TransformComponent>());

	const_cast<TransformComponent&>(parentEntity.Get<TransformComponent>()).AttachChild(mEntity);
}

void TransformComponent::DetachFromParent()
{
	if (mEntity.IsValid() && mParent.IsValid())
	{
		enAssert(mParent.Has<TransformComponent>());
		const_cast<TransformComponent&>(mParent.Get<TransformComponent>()).DetachChild(mEntity);
	}
}

bool TransformComponent::HasParent() const
{
	return mParent.IsValid();
}

Entity TransformComponent::GetParent() const
{
	return mParent;
}

const Matrix4f& TransformComponent::GetGlobalMatrix() const
{
	if (mGlobalMatrixDirty)
	{
		UpdateGlobalMatrix();
	}
	return mGlobalMatrix;
}

Vector3f TransformComponent::GetGlobalPosition() const
{
	return GetGlobalMatrix().GetTranslation();
}

Matrix3f TransformComponent::GetGlobalRotation() const
{
	return GetGlobalMatrix().GetRotation();
}

const Matrix4f& TransformComponent::GetLocalMatrix() const
{
	return mMatrix;
}

void TransformComponent::MarkGlobalMatrixAsDirty()
{
	mGlobalMatrixDirty = true;

	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		if (mChildren[i].IsValid())
		{
			enAssert(mChildren[i].Has<TransformComponent>());
			mChildren[i].Get<TransformComponent>().MarkGlobalMatrixAsDirty();
		}
	}
}

bool TransformComponent::Initialize(const Entity& entity)
{
	enAssert(entity.IsValid());
	mEntity = entity;
	return true;
}

bool TransformComponent::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<TransformComponent>::GetName(), TypeInfo<TransformComponent>::GetHash()))
	{
		bool ret = true;
		ret = GenericSerialization(serializer, "Transform", static_cast<Transform&>(*this)) && ret;
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool TransformComponent::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<TransformComponent>::GetName(), TypeInfo<TransformComponent>::GetHash()))
	{
		bool ret = true;
		ret = GenericEdit(objectEditor, "Transform", static_cast<Transform&>(*this)) || ret;
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

void TransformComponent::UpdateGlobalMatrix() const
{
	if (mParent.IsValid())
	{
		enAssert(mParent.Has<TransformComponent>());
		mGlobalMatrix = mMatrix * mParent.Get<TransformComponent>().GetGlobalMatrix();
	}
	else
	{
		mGlobalMatrix = mMatrix;
	}
	mGlobalMatrixDirty = false;
}

} // namespace en