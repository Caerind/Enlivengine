#pragma once

#include <Enlivengine/Graphics/View.hpp>

#include <Enlivengine/Core/ComponentTraits.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

class World;

class ViewComponent : public View
{
public:
	ViewComponent();
	~ViewComponent();

	ViewComponent(ViewComponent&& other) noexcept;
	ViewComponent(const ViewComponent&) = delete;

	ViewComponent& operator=(ViewComponent&& other) noexcept;
	ViewComponent& operator=(const ViewComponent&) = delete;

	bool Initialize(const Entity& entity);
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	friend struct ComponentHasCustomInitialization<ViewComponent>;

	Entity mEntity;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::ViewComponent, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()
ENLIVE_COMPONENT_CUSTOM_INITIALIZATION(en::ViewComponent)