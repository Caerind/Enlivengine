#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Meta/MetaSpecialization.hpp>

namespace en
{

ImGuiEntityBrowser::ImGuiEntityBrowser()
	: ImGuiTool()
{
}

ImGuiToolTab ImGuiEntityBrowser::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiEntityBrowser::GetName() const
{
	return ICON_FA_USER_COG " EntityBrowser";
}

const char* ImGuiEntityBrowser::GetSaveName() const
{
	return "EntityBrowser";
}

void ImGuiEntityBrowser::Display()
{
	if (World* world = Universe::GetInstance().GetCurrentWorld())
	{
		static constexpr char unknownEntityName[] = "<Unknown>";

		static constexpr U32 entityNameMaxSize = 255;
		static char entityNameInput[entityNameMaxSize] = "";

		EntityManager& entityManager = world->GetEntityManager();
		bool button = ImGui::Button("New Entity");
		ImGui::SameLine();
		bool input = ImGui::InputText("", entityNameInput, entityNameMaxSize, ImGuiInputTextFlags_EnterReturnsTrue);
		if (button || input)
		{
			Entity newEntity = entityManager.CreateEntity();
			if (strlen(entityNameInput) > 0)
			{
				if (newEntity.IsValid())
				{
					newEntity.Add<NameComponent>(entityNameInput);

					world->SelectEntity(newEntity);
				}
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(entityNameInput, "");
#else
				strcpy(entityNameInput, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}

		entityManager.Each([&world](auto entityEntt)
		{
			Entity entity(world->GetEntityManager(), entityEntt);
			if (entity.IsValid())
			{
				ImGui::PushID(entity.GetID());
				if (ImGui::Button("x"))
				{
					world->GetEntityManager().DestroyEntity(entity);
				}
				else
				{
					ImGui::SameLine();
					const char* entityName = entity.GetName();
					if (entityName == nullptr || strlen(entityName) == 0)
					{
						entityName = unknownEntityName;
					}
					if (ImGui::Button(entityName))
					{
						world->SelectEntity(entity);
					}
				}
				ImGui::PopID();
			}
		});

		const auto& selectedEntities = world->GetSelectedEntities();
		U32 selectedEntitiesCount = static_cast<U32>(selectedEntities.size());
		for (U32 i = 0; i < selectedEntitiesCount; )
		{
			Entity entity(world->GetEntityManager(), selectedEntities[i]);
			bool selected = true;
			if (entity.IsValid())
			{
				const char* entityName = entity.GetName();
				if (entityName == nullptr || strlen(entityName) == 0)
				{
					entityName = unknownEntityName;
				}
				if (ImGui::Begin(entityName, &selected) && HasCustomEditor<en::Entity>::value)
				{
					HasCustomEditor<en::Entity>::ImGuiEditor(entity, nullptr);
					ImGui::End();
				}
				else
				{
					ImGui::End();
					selected = false;
				}
			}
			else
			{
				selected = false;
			}

			if (!selected)
			{
				if (world->UnselectEntity(entity))
				{
					selectedEntitiesCount--;
				}
				else
				{
					i++;
				}
			}
			else
			{
				i++;
			}
		}
	}
	else
	{
		ImGui::Text("No CurrentWorld for EntityBrowser");
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
