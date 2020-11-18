#include <Enlivengine/Tools/ImGuiTool.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

namespace en
{

ImGuiTool::ImGuiTool()
	: mVisible(false)
{
}

void ImGuiTool::Initialize()
{
}

void ImGuiTool::Release()
{
}

void ImGuiTool::Display()
{
}

int ImGuiTool::GetWindowFlags() const
{
	return ImGuiWindowFlags_None;
}

bool ImGuiTool::IsImGuiDemoTool() const
{
	return false;
}

bool ImGuiTool::IsVisible() const
{
	return mVisible;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS