#include <Enlivengine/Tools/ImGuiPhysic.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/Universe.hpp>

#include <Enlivengine/Core/EntitySpecialization.hpp>
#include <Enlivengine/Core/EntityManagerSpecialization.hpp>

namespace en
{

ImGuiPhysic::ImGuiPhysic()
	: ImGuiTool()
{
}

ImGuiPhysic::~ImGuiPhysic()
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

void ImGuiPhysic::Display()
{
	if (World* world = Universe::GetInstance().GetCurrentWorld())
	{
		if (world->HasPhysicSystem())
		{
			PhysicSystem* physicSystem = world->GetPhysicSystem();
			if (physicSystem->IsPlaying())
			{
				if (ImGui::Button("Pause"))
				{
					physicSystem->Pause();
				}
			}
			else
			{
				if (ImGui::Button("Play"))
				{
					physicSystem->Play();
				}
			}
			auto gravity = physicSystem->GetGravity();
			if (ObjectEditor::ImGuiEditor(gravity, "Gravity"))
			{
				physicSystem->SetGravity(gravity);
			}
			auto ppm = physicSystem->GetPixelsPerMeter();
			if (ObjectEditor::ImGuiEditor(ppm, "PixelsPerMeter"))
			{
				physicSystem->SetPixelsPerMeter(ppm);
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
