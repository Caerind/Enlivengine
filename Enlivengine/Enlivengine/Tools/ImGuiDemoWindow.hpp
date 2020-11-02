#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiToolManager.hpp>

namespace en
{

class ImGuiDemoWindow : public ImGuiTool
{
public:
	static ImGuiDemoWindow& GetInstance();

	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

	virtual bool IsImGuiDemoTool() const;

private:
	ImGuiDemoWindow();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
