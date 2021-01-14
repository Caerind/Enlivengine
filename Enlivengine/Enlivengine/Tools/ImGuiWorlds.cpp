#include <Enlivengine/Tools/ImGuiWorlds.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Resources/PathManager.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>

#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/SystemFactory.hpp>

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
	// Current World
	if (ImGui::CollapsingHeader("Current World"))
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
				Universe::UnloadCurrentWorld();
				ImGui::Unindent();
				return;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Unload World");
			}

			if (ImGui::CollapsingHeader("Systems"))
			{
				ImGui::Indent();

				const auto& systemInfos = SystemFactory::GetSystemInfos();
				static std::vector<U32> hasNot;
				hasNot.clear();
				const auto endItr = systemInfos.cend();
				for (auto itr = systemInfos.cbegin(); itr != endItr; ++itr)
				{
					const auto& si = itr->second;
					if (si.has(world))
					{
						ImGui::PushID(itr->first);
						if (ImGui::Button("-"))
						{
							si.remove(world);
							worldModified = true;
							ImGui::PopID();
							continue;
						}
						else
						{
							ImGui::SameLine();
							ImGui::Text("%s", itr->second.name);
							ImGui::PopID();
						}
					}
					else
					{
						hasNot.push_back(itr->first);
					}
				}

				if (!hasNot.empty())
				{
					if (ImGui::Button("+ Add System"))
					{
						ImGui::OpenPopup("Add System");
					}
					if (ImGui::BeginPopup("Add System"))
					{
						ImGui::TextUnformatted("Available:");
						ImGui::Separator();
						for (auto systemHash : hasNot)
						{
							const auto& si = systemInfos.at(systemHash);
							ImGui::PushID(systemHash);
							if (ImGui::Selectable(si.name))
							{
								si.add(world);
								worldModified = true;
							}
							ImGui::PopID();
						}
						ImGui::EndPopup();
					}
				}

				ImGui::Unindent();
			}

			if (worldModified)
			{
				// TODO : World Modified
				//world.SaveToFile();
			}
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

				Universe::CreateWorld(worldName);

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
					Universe::LoadWorld(mWorlds[i]);
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Load World");
				}

				ImGui::SameLine();

				if (ImGui::Button(ICON_FA_TRASH))
				{
					Universe::RemoveWorld(mWorlds[i]);
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
		DataFile xml;
		if (!xml.LoadFromFile(path.string()))
		{
			return false;
		}
		if (!xml.Deserialize(mWorlds, "Worlds"))
		{
			return false;
		}

		std::string currentWorldName = "";
		if (xml.Deserialize(currentWorldName, "CurrentWorld"))
		{
			if (currentWorldName != "")
			{
				Universe::LoadWorld(currentWorldName);
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

	DataFile xml;
	xml.CreateEmptyFile();
	xml.Serialize(mWorlds, "Worlds");
	if (World* world = Universe::GetCurrentWorld())
	{
		xml.Serialize(world->GetName(), "CurrentWorld");
	}
	else
	{
		std::string emptyString = "";
		xml.Serialize(emptyString, "CurrentWorld");
	}
	return xml.SaveToFile(path.string());
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI