#include <Enlivengine/Tools/ImGuiPhysic.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/PhysicSystem2D.hpp>

#include <Enlivengine/Tools/ImGuiObjectEditor.hpp>

namespace en
{

ImGuiPhysic::ImGuiPhysic()
	: ImGuiTool()
{
}

ImGuiToolTab ImGuiPhysic::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiPhysic::GetName() const
{
	return ICON_FA_METEOR " Physic";
}

const char* ImGuiPhysic::GetSaveName() const
{
	return "Physic";
}

void ImGuiPhysic::Display()
{
	if (World* world = Universe::GetCurrentWorld())
	{
		if (world->HasPhysicSystem())
		{
			if (PhysicSystem2D* physicSystem = dynamic_cast<PhysicSystem2D*>(world->GetPhysicSystem()))
			{
				ImGuiObjectEditor editor;
				physicSystem->Edit(editor, "PhysicSystem2D");
			}
		}
		else
		{
			ImGui::Text("CurrentWorld doesn't have a Physic system");
		}
	}
	else
	{
		ImGui::Text("No CurrentWorld for Physic");
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
