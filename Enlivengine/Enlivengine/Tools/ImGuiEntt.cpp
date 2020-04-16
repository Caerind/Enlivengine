#include <Enlivengine/Tools/ImGuiEntt.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

namespace en
{

ImGuiEntt& ImGuiEntt::GetInstance()
{
	static ImGuiEntt instance;
	return instance;
}

ImGuiToolTab ImGuiEntt::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiEntt::GetName() const
{
	return ICON_FA_ATOM " Entt";
}

void ImGuiEntt::Display()
{
	ImGui::Text("Test");
	if (ImGui::Button("Test"))
	{
		ImGui::Text("Ah");
	}
}

ImGuiEntt::ImGuiEntt()
	: ImGuiTool()
{
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
