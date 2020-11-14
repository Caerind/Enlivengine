#include <Enlivengine/Core/TransformComponent.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Core/Components.hpp>

namespace en
{

TransformComponent::TransformComponent()
	: Transform()
	, mEntity()
{
}

TransformComponent::~TransformComponent()
{
}

TransformComponent::TransformComponent(TransformComponent&& other) noexcept
	: Transform(other)
	, mEntity(other.mEntity)
{
	other.mEntity = Entity();
}

TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
{
	if (&other != this)
	{
		Transform::operator=(other);
		mEntity = other.mEntity;
		other.mEntity = Entity();
	}
	return *this;
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