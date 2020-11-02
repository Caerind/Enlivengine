#include <Enlivengine/Tools/ImGuiDemoWindow.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <dear-imgui/imgui.h>

namespace en
{

ImGuiDemoWindow& ImGuiDemoWindow::GetInstance()
{
	static ImGuiDemoWindow instance;
	return instance;
}

ImGuiToolTab ImGuiDemoWindow::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiDemoWindow::GetName() const
{
	return /*ICON_FA_INFO_CIRCLE*/ " ImGui Demo"; // TODO : Restore FontAwesome
}

void ImGuiDemoWindow::Display()
{
	ImGui::ShowDemoWindow(&mVisible);
}

bool ImGuiDemoWindow::IsImGuiDemoTool() const
{
	return true;
}

ImGuiDemoWindow::ImGuiDemoWindow()
	: ImGuiTool()
{
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
