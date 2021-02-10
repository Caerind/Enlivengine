#include <Enlivengine/Tools/ImGuiWorlds.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Resources/PathManager.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/SystemFactory.hpp>
#include <Enlivengine/Engine/WorldFileManager.hpp>

#include <Enlivengine/Utils/XmlSerializer.hpp>
#include <Enlivengine/Tools/ImGuiObjectEditor.hpp>
#include <Enlivengine/Tools/ImGuiEditor.hpp>

namespace en
{

ImGuiWorlds::ImGuiWorlds()
	: ImGuiTool()
{
}

ImGuiToolTab ImGuiWorlds::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiWorlds::GetName() const
{
	return ICON_FA_GLOBE_EUROPE " Worlds";
}

const char* ImGuiWorlds::GetSaveName() const
{
	return "Worlds";
}

void ImGuiWorlds::Initialize()
{
	LoadWorldsFromFile();
}

void ImGuiWorlds::Display()
{
	CurrentWorld();

	ImGui::Separator();

	AllWorlds();
}

void ImGuiWorlds::CurrentWorld()
{
	ImGui::Text("Current World");
	{
		ImGui::Indent();
		if (World* worldPtr = Universe::GetCurrentWorld())
		{
			bool worldModified = false;
			World& world = *worldPtr;

			ImGui::Text("%s", world.GetName().c_str());

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_UPLOAD))
			{
				WorldFileManager::UnloadCurrentWorld();
				ImGui::Unindent();
				return;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Unload World");
			}

			ImGuiObjectEditor systemEditor;
			worldModified = GenericEdit(systemEditor, "Systems", world.GetSystemManager());

#ifdef ENLIVE_TOOL
			if (worldModified && ImGuiEditor::IsStopped())
			{
				WorldFileManager::SaveCurrentWorld();
			}
#else
			ENLIVE_UNUSED(worldModified);
#endif // ENLIVE_TOOL
		}
		else
		{
			ImGui::Text("No world loaded");
		}
		ImGui::Unindent();
	}
}

void ImGuiWorlds::AllWorlds()
{
	bool modified = false;

	// New World
	ImGui::Text("Create New World");
	{
		ImGui::Indent();

		static constexpr U32 kBufferSize{ 256 };
		static char newWorldName[kBufferSize] = "";
		ImGui::InputText("Name##NewWorld", newWorldName, kBufferSize);

		bool validNewInput = true;
		const U32 nameLength = static_cast<U32>(strlen(newWorldName));
		if (nameLength <= 0 || nameLength > kBufferSize)
		{
			validNewInput = false;
		}

		if (validNewInput)
		{
			if (ImGui::Button("Create World##NewWorld"))
			{
				const std::string worldName = std::string(newWorldName);
				mWorlds.push_back(worldName);

				WorldFileManager::CreateWorld(worldName);

				modified = true;

#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(newWorldName, "");
#else
				strcpy(newWorldName, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}
		else
		{
			ImGui::DisabledButton("Create World##NewWorld");
		}
		ImGui::Unindent();
	}

	ImGui::Separator();

	// World list
	ImGui::Text("Worlds list");
	{
		ImGui::Indent();
		U32 worldCount = static_cast<U32>(mWorlds.size());
		for (U32 i = 0; i < worldCount; )
		{
			bool remove = false;

			ImGui::Text("%s", mWorlds[i].c_str());
			ImGui::PushID(i);
			ImGui::PushID(mWorlds[i].c_str());

			ImGui::SameLine();
			if (Universe::GetCurrentWorld() != nullptr && Universe::GetCurrentWorld()->GetName() == mWorlds[i])
			{
				ImGui::DisabledButton(ICON_FA_DOWNLOAD);
				ImGui::SameLine();
				ImGui::DisabledButton(ICON_FA_TRASH);
			}
			else
			{
				if (ImGui::Button(ICON_FA_DOWNLOAD))
				{
					WorldFileManager::LoadWorld(mWorlds[i]);
					modified = true;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Load World");
				}

				ImGui::SameLine();

				if (ImGui::Button(ICON_FA_TRASH))
				{
					WorldFileManager::RemoveWorld(mWorlds[i]);
					remove = true;
					modified = true;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Remove World");
				}
			}

			if (remove)
			{
				mWorlds.erase(mWorlds.begin() + i);
				worldCount--;
			}
			else
			{
				++i;
			}

			ImGui::PopID();
			ImGui::PopID();
		}
		ImGui::Unindent();
	}

	if (modified)
	{
		SaveWorldsToFile();
	}
}

bool ImGuiWorlds::LoadWorldsFromFile()
{
	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + "worlds.data");
	if (std::filesystem::exists(path))
	{
		XmlSerializer xml;
		if (!xml.Open(path.generic_string(), Serializer::Mode::Read))
		{
			return false;
		}

		if (!GenericSerialization(xml, "Worlds", mWorlds))
		{
			enLogError(LogChannel::Tools, "Can't load Worlds");
			return false;
		}

		std::string currentWorldName = "";
		if (GenericSerialization(xml, "CurrentWorld", currentWorldName))
		{
			if (currentWorldName != "")
			{
				WorldFileManager::LoadWorld(currentWorldName);
			}
		}

		return true;
	}
	else
	{
		return SaveWorldsToFile();
	}
}

bool ImGuiWorlds::SaveWorldsToFile()
{
	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + "worlds.data");

	XmlSerializer xml;
	if (xml.Open(path.generic_string(), Serializer::Mode::Write))
	{
		GenericSerialization(xml, "Worlds", mWorlds);

		const World* world = Universe::GetCurrentWorld();
		GenericSerialization(xml, "CurrentWorld", (world != nullptr) ? world->GetName() : "");

		return xml.Close();
	}
	else
	{
		enLogError(LogChannel::Tools, "Can't save Worlds");
		return false;
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI