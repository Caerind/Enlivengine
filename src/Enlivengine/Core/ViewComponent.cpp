#include <Enlivengine/Core/ViewComponent.hpp>

#include <Enlivengine/Core/TransformComponent.hpp>

namespace en
{

ViewComponent::ViewComponent()
	: View()
	, mEntity()
{
}

ViewComponent::~ViewComponent()
{
}

ViewComponent::ViewComponent(ViewComponent&& other) noexcept
	: View()
	, mEntity(other.mEntity)
{
	other.mEntity = Entity();
}

ViewComponent& ViewComponent::operator=(ViewComponent&& other) noexcept
{
	mEntity = other.mEntity;
	other.mEntity = Entity();
	return *this;
}

bool ViewComponent::Initialize(const Entity& entity)
{
	enAssert(entity.IsValid());
	mEntity = entity;
	return true;
}

bool ViewComponent::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<ViewComponent>::GetName(), TypeInfo<ViewComponent>::GetHash()))
	{
		bool ret = true;
		ret = View::Serialize(serializer, "View") && ret;
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool ViewComponent::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<ViewComponent>::GetName(), TypeInfo<ViewComponent>::GetHash()))
	{
		bool ret = false;
		ret = View::Edit(objectEditor, "View") || ret;
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

} // namespace en