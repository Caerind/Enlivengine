#include <Enlivengine/Core/TransformComponent.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Core/Components.hpp>

namespace en
{

TransformComponent::TransformComponent()
	: Transform()
	, mEntity()
	, mParent()
	, mChildren()
{
}

TransformComponent::~TransformComponent()
{
	DetachFromParent();
}

TransformComponent::TransformComponent(TransformComponent&& other) noexcept
	: Transform(other)
	, mEntity(other.mEntity)
	, mParent(other.mParent)
	, mChildren(std::move(other.mChildren))
{
	other.mEntity = Entity();
	other.mParent = Entity();
}

TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
{
	if (&other != this)
	{
		Transform::operator=(other);
		mEntity = other.mEntity;
		mParent = other.mParent;
		mChildren = std::move(other.mChildren);
		other.mEntity = Entity();
		other.mParent = Entity();
	}
	return *this;
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

	// Set the new parent
	childTransform.mParent = mEntity;

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

	// Reset the parent
	childTransform.mParent = Entity();

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

Matrix4f TransformComponent::GetGlobalMatrix() const
{
	if (mParent.IsValid())
	{
		enAssert(mParent.Has<TransformComponent>());
		return mParent.Get<TransformComponent>().GetGlobalMatrix() * mMatrix;
	}
	else
	{
		return mMatrix;
	}
}

Vector3f TransformComponent::GetGlobalPosition() const
{
	return GetGlobalMatrix().GetTranslation();
}

Matrix4f& TransformComponent::GetLocalMatrix()
{
	return mMatrix;
}

const en::Matrix4f& TransformComponent::GetLocalMatrix() const
{
	return mMatrix;
}

Entity TransformComponent::GetEntity() const
{
	return mEntity;
}

World* TransformComponent::GetWorld()
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

const World* TransformComponent::GetWorld() const
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

} // namespace en

#endif // ENLIVE_MODULE_CORE