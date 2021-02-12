#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Core/Components.hpp>

namespace en
{

TransformComponent::TransformComponent()
	: mLocalTransform()
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
	: mLocalTransform(other.mLocalTransform)
	, mGlobalMatrix(other.mGlobalMatrix)
	, mEntity(other.mEntity)
	, mParent(other.mParent)
	, mChildren(std::move(other.mChildren))
	, mGlobalMatrixDirty(other.mGlobalMatrixDirty)
{
	other.mEntity = Entity();
	other.mParent = EntityHandle();
	other.mChildren.clear();
}

TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
{
	if (&other != this)
	{
		mLocalTransform = other.mLocalTransform;
		mGlobalMatrix = other.mGlobalMatrix;
		mEntity = other.mEntity;
		mParent = other.mParent;
		mChildren = std::move(other.mChildren);
		mGlobalMatrixDirty = other.mGlobalMatrixDirty;
		other.mEntity = Entity();
		other.mParent = EntityHandle();
		other.mChildren.clear();
	}
	return *this;
}

Vector3f TransformComponent::GetPosition() const
{
	return mLocalTransform.GetPosition();
}

void TransformComponent::SetPosition(const Vector3f& position)
{
	mLocalTransform.SetPosition(position);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Move(const Vector3f& movement)
{
	mLocalTransform.Move(movement);
	MarkGlobalMatrixAsDirty();
}

Matrix3f TransformComponent::GetRotation() const
{
	return mLocalTransform.GetRotation();
}

void TransformComponent::SetRotation(const Matrix3f& rotation)
{
	mLocalTransform.SetRotation(rotation);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Rotate(const Matrix3f& rotation)
{
	mLocalTransform.Rotate(rotation);
	MarkGlobalMatrixAsDirty();
}

Vector3f TransformComponent::GetScale() const
{
	return mLocalTransform.GetScale();
}

void TransformComponent::SetScale(const Vector3f& scale)
{
	mLocalTransform.SetScale(scale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::SetUniformScale(F32 uniformScale)
{
	mLocalTransform.SetUniformScale(uniformScale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Scale(const Vector3f& scale)
{
	mLocalTransform.Scale(scale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::Scale(F32 uniformScale)
{
	mLocalTransform.Scale(uniformScale);
	MarkGlobalMatrixAsDirty();
}

void TransformComponent::SetTransform(const Vector3f& translation, const Matrix3f& rotation, const Vector3f& scale /*= Vector3f(1.0f)*/)
{
	mLocalTransform.SetTransform(translation, rotation, scale);
	MarkGlobalMatrixAsDirty();
}

bool TransformComponent::CanAttach(const Entity& childEntity) const
{
	if (mEntity != childEntity && mEntity.IsValid() && childEntity.IsValid() && childEntity.Has<TransformComponent>())
	{
		return !IsChildOrSubchild(childEntity);
	}
	else
	{
		return false;
	}
}

void TransformComponent::AttachChild(const Entity& childEntity)
{
	enAssert(childEntity != mEntity);
	enAssert(mEntity.IsValid());
	enAssert(childEntity.IsValid());
	enAssert(childEntity.Has<TransformComponent>());

	enAssert(CanAttach(childEntity));

	TransformComponent& childTransform = const_cast<TransformComponent&>(childEntity.Get<TransformComponent>());

	// Skip if already the parent
	Entity parentEntity = childTransform.mParent.Get();
	if (parentEntity == mEntity)
	{
		return;
	}

	// Detach the current parent of the child
	if (parentEntity.IsValid())
	{
		enAssert(parentEntity.Has<TransformComponent>());
		enAssert(parentEntity.Get<TransformComponent>().IsChild(childEntity));
		parentEntity.Get<TransformComponent>().DetachChild(childEntity);
	}

	// Set the new parent & matrix
	const Matrix4f result = childTransform.GetGlobalMatrix() * GetGlobalMatrix().Inversed();
	childTransform.mParent.Set(mEntity);
	childTransform.SetTransform(result.GetTranslation(), result.GetRotation(), result.GetScale());

	// Store the child on parent's side
	mChildren.push_back(EntityHandle(childEntity));
}

void TransformComponent::DetachChild(const Entity& childEntity)
{
	enAssert(mEntity.IsValid());
	enAssert(childEntity.IsValid());
	enAssert(childEntity.Has<TransformComponent>());
	enAssert(IsChild(childEntity));
	TransformComponent& childTransform = const_cast<TransformComponent&>(childEntity.Get<TransformComponent>());

	// It should be the parent
	Entity parentEntity = childTransform.mParent.Get();
	enAssert(parentEntity == mEntity);

	// Reset the parent & matrix
	const Matrix4f result = childTransform.GetGlobalMatrix();
	childTransform.mParent = EntityHandle();
	childTransform.SetTransform(result.GetTranslation(), result.GetRotation(), result.GetScale());

	// Remove the child on parent's side
	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		Entity child = mChildren[i].Get();
		if (child == childEntity)
		{
			mChildren.erase(mChildren.begin() + i);
			return;
		}
	}
	enAssert(false); // Was not a stored child on parent's side
}

bool TransformComponent::IsChild(const Entity& childEntity) const
{
	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		Entity child = mChildren[i].Get();
		if (child.IsValid() && child == childEntity)
		{
			return true;
		}
	}
	return false;
}

bool TransformComponent::IsChildOrSubchild(const Entity& childEntity) const
{
	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		Entity child = mChildren[i].Get();
		if (child.IsValid())
		{
			if (child == childEntity)
			{
				return true;
			}
			else
			{
				const TransformComponent& childTransform = child.Get<TransformComponent>();
				if (childTransform.IsChildOrSubchild(childEntity))
				{
					return true;
				}
			}
		}
	}
	return false;
}

U32 TransformComponent::GetChildrenCount() const
{
	return static_cast<U32>(mChildren.size());
}

const EntityHandle& TransformComponent::GetChild(U32 index) const
{
	enAssert(index < GetChildrenCount());
	return mChildren[index];
}

void TransformComponent::AttachToParent(const Entity& parentEntity)
{
	enAssert(parentEntity.IsValid());
	enAssert(parentEntity.Has<TransformComponent>());
	const_cast<TransformComponent&>(parentEntity.Get<TransformComponent>()).AttachChild(mEntity);
	enAssert(parentEntity.Get<TransformComponent>().IsChild(mEntity));
	enAssert(mParent.IsValid());
	enAssert(mParent.Get().IsValid());
	enAssert(mParent.Get() == parentEntity);
}

void TransformComponent::DetachFromParent()
{
	Entity parentEntity = mParent.Get();
	if (mEntity.IsValid() && parentEntity.IsValid())
	{
		enAssert(parentEntity.Has<TransformComponent>());
		enAssert(parentEntity.Get<TransformComponent>().IsChild(mEntity));
		const_cast<TransformComponent&>(parentEntity.Get<TransformComponent>()).DetachChild(mEntity);
		enAssert(!mParent.IsValid());
		enAssert(!parentEntity.Get<TransformComponent>().IsChild(mEntity));
	}
}

bool TransformComponent::HasParent() const
{
	return mParent.IsValid();
}

const EntityHandle& TransformComponent::GetParent() const
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
	return mLocalTransform.GetMatrix();
}

void TransformComponent::MarkGlobalMatrixAsDirty()
{
	mGlobalMatrixDirty = true;

	const U32 childrenCount = GetChildrenCount();
	for (U32 i = 0; i < childrenCount; ++i)
	{
		Entity child = mChildren[i].Get();
		if (child.IsValid())
		{
			enAssert(child.Has<TransformComponent>());
			child.Get<TransformComponent>().MarkGlobalMatrixAsDirty();
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
		ret = GenericSerialization(serializer, "transform", mLocalTransform) && ret;
		ret = GenericSerialization(serializer, "parent", mParent) && ret;
		ret = GenericSerialization(serializer, "children", mChildren) && ret;
		ret = serializer.EndClass() && ret;

		if (serializer.IsReading())
		{
			enAssert(mEntity.HasManager());
			EntityManager& entityManager = mEntity.GetManager();
			mParent.Set(entityManager, mParent.GetUID());
			for (auto& childHandle : mChildren)
			{
				childHandle.Set(entityManager, childHandle.GetUID());
			}

			MarkGlobalMatrixAsDirty();
		}

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
		bool ret = false;
		ret = GenericEdit(objectEditor, "Transform", mLocalTransform) || ret;

#ifdef ENLIVE_ENABLE_IMGUI
		if (objectEditor.IsImGuiEditor())
		{
			if (ImGui::CollapsingHeader("Parent&Children"))
			{
				ImGui::Indent();
#endif // ENLIVE_ENABLE_IMGUI

				ret = GenericEdit(objectEditor, "Parent", mParent) || ret;
				ret = GenericEdit(objectEditor, "Children", mChildren) || ret;

#ifdef ENLIVE_ENABLE_IMGUI
				ImGui::Unindent();
			}
		}
#endif // ENLIVE_ENABLE_IMGUI

		objectEditor.EndClass();

		if (ret)
		{
			MarkGlobalMatrixAsDirty();
		}

		return ret;
	}
	else
	{
		return false;
	}
}

const Entity& TransformComponent::GetEntity() const
{
	return mEntity;
}

void TransformComponent::UpdateGlobalMatrix() const
{
	Entity parentEntity = mParent.Get();
	if (parentEntity.IsValid())
	{
		enAssert(parentEntity.Has<TransformComponent>());
		mGlobalMatrix = mLocalTransform.GetMatrix() * parentEntity.Get<TransformComponent>().GetGlobalMatrix();
	}
	else
	{
		mGlobalMatrix = mLocalTransform.GetMatrix();
	}
	mGlobalMatrixDirty = false;
}

} // namespace en