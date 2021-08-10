#include <Enlivengine/Core/UIDComponent.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

UIDComponent::UIDComponent()
	: mUID(kInvalidUID)
{
}

UIDComponent::~UIDComponent()
{
}

UIDComponent::UIDComponent(UIDComponent&& other) noexcept
	: mUID(other.mUID)
{
	other.mUID = kInvalidUID;
}

UIDComponent& UIDComponent::operator=(UIDComponent&& other) noexcept
{
	mUID = other.mUID;
	other.mUID = kInvalidUID;
	return *this;
}

UID UIDComponent::GetUID() const
{
	return mUID;
}

bool UIDComponent::Serialize(Serializer& serializer, const char* name)
{
	return serializer.Serialize(name, mUID);
}

bool UIDComponent::Edit(ObjectEditor& objectEditor, const char* name)
{
	ENLIVE_UNUSED(name);
	if (objectEditor.IsImGuiEditor())
	{
#ifdef ENLIVE_ENABLE_IMGUI
		ImGui::Text("%s: %d", name, mUID);
#endif // ENLIVE_ENABLE_IMGUI
	}
	return false;
}

} // namespace en