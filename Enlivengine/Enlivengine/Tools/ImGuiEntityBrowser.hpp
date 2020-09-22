#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>

#include <Enlivengine/Core/Entity.hpp>

namespace en
{

class ImGuiEntityBrowser : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiEntityBrowser);
	~ImGuiEntityBrowser();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

	bool IsSelected(const Entity& entity) const;

private:
	std::vector<entt::entity> mSelectedEntities;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
