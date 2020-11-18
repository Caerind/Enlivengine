#include <Enlivengine/Tools/ImGuiDemoWindow.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

namespace en
{

ImGuiDemoWindow::ImGuiDemoWindow()
	: ImGuiTool()
{
}

ImGuiToolTab ImGuiDemoWindow::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiDemoWindow::GetName() const
{
	return /*ICON_FA_INFO_CIRCLE*/ " ImGui Demo"; // TODO : Restore FontAwesome
}

const char* ImGuiDemoWindow::GetSaveName() const
{
	return "ImGuiDemo";
}

void ImGuiDemoWindow::Display()
{
	ImGui::ShowDemoWindow(&mVisible);
}

bool ImGuiDemoWindow::IsImGuiDemoTool() const
{
	return true;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
