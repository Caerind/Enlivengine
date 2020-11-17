#include <Enlivengine/Tools/ImGuiInputEditor.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Window/EventSystem.hpp>

namespace en
{

ImGuiInputEditor::ImGuiInputEditor()
	: ImGuiTool()
{
}

ImGuiToolTab ImGuiInputEditor::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiInputEditor::GetName() const
{
	return /*ICON_FA_GAMEPAD*/ " InputEditor"; // TODO : Restore FontAwesome
}

void ImGuiInputEditor::Display()
{
	/*
	
	// TOOD : TOOLS UPDATE

	*/
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
