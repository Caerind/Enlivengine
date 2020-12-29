#include <Enlivengine/Tools/ImGuiPhysic.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/Engine.hpp>

#include <Enlivengine/Meta/ObjectEditor.hpp>

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
	if (World* world = Engine::GetCurrentWorld())
	{
		if (world->HasPhysicSystem())
		{
			PhysicSystem* physicSystem = world->GetPhysicSystem();
			auto gravity = physicSystem->GetGravity();
			if (ObjectEditor::ImGuiEditor(gravity, "Gravity"))
			{
				physicSystem->SetGravity(gravity);
			}
			auto velocityIterations = physicSystem->GetVelocityIterations();
			if (ObjectEditor::ImGuiEditor(velocityIterations, "VelocityIterations"))
			{
				physicSystem->SetVelocityIterations(velocityIterations);
			}
			auto positionIterations = physicSystem->GetPositionIterations();
			if (ObjectEditor::ImGuiEditor(positionIterations, "PositionIterations"))
			{
				physicSystem->SetPositionIterations(positionIterations);
			}
#ifdef ENLIVE_DEBUG
			if (physicSystem->IsDebugRendering())
			{
				if (ImGui::Button("HideDebug"))
				{
					physicSystem->SetDebugRendering(false);
				}	
			}
			else
			{
				if (ImGui::Button("ShowDebug"))
				{
					physicSystem->SetDebugRendering(true);
				}
			}
			if (physicSystem->IsDebugRendering())
			{
				auto renderFlags = physicSystem->GetDebugRenderFlags();
				// TODO : DebugRenderFlags
				{
					physicSystem->SetDebugRenderFlags(renderFlags);
				}
			}
#endif // ENLIVE_DEBUG
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
