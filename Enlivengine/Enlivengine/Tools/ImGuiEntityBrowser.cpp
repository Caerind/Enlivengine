#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui_internal.h>

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Window/Keyboard.hpp>

namespace en
{

static constexpr char kUnknownEntityName[] = "<Unknown>";

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
	if (World* world = Universe::GetCurrentWorld())
	{
		static constexpr U32 entityNameMaxSize = 255;
		static char entityNameInput[entityNameMaxSize] = "";

		EntityManager& entityManager = world->GetEntityManager();
		bool button = ImGui::Button("New Entity");
		ImGui::SameLine();
		bool input = ImGui::InputText("##entityNameInput", entityNameInput, entityNameMaxSize, ImGuiInputTextFlags_EnterReturnsTrue);
		if (button || input)
		{
			Entity newEntity = entityManager.CreateEntity();
			if (strlen(entityNameInput) > 0)
			{
				if (newEntity.IsValid())
				{
					newEntity.Add<NameComponent>(entityNameInput);

					world->ClearSelectedEntities();
					world->SelectEntity(newEntity);
				}
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(entityNameInput, "");
#else
				strcpy(entityNameInput, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}

		entityManager.Each([&world, this](auto entityEntt)
		{
			Entity entity(world->GetEntityManager(), entityEntt);
			if (entity.IsValid())
			{
				if (!entity.Has<TransformComponent>() || !entity.Get<TransformComponent>().HasParent())
				{
					DisplayEntity(entity, world);
				}
			}
		});

		const ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		const ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 dndMin = ImVec2(vMin.x + windowPos.x, vMin.y + windowPos.y);
		const ImVec2 dndMax = ImVec2(vMax.x + windowPos.x, vMax.y + windowPos.y);
		ImRect dndArea(dndMin, dndMax);
		if (ImGui::BeginDragDropTargetCustom(dndArea, ImGui::GetCurrentWindow()->ID))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG_N_DROP"))
			{
				enAssert(payload->DataSize == sizeof(Entity));
				Entity receivedEntity = *(Entity*)payload->Data;
				enAssert(receivedEntity.Has<TransformComponent>());
				receivedEntity.Get<TransformComponent>().DetachFromParent();
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::Begin("EntityEditor"))
		{
			const auto& selectedEntities = world->GetSelectedEntities();
			U32 selectedEntitiesCount = static_cast<U32>(selectedEntities.size());
			if (selectedEntitiesCount > 0)
			{
				for (U32 i = 0; i < selectedEntitiesCount; )
				{
					Entity entity(world->GetEntityManager(), selectedEntities[i]);
					bool selected = true;
					if (entity.IsValid())
					{
						const char* entityName = entity.GetName();
						if (entityName == nullptr || strlen(entityName) == 0)
						{
							entityName = kUnknownEntityName;
						}

						enAssert(false);
						// TODO : ObjectEditor
						/*
						ObjectEditor::ImGuiEditor(entity, entityName);
						*/
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
				ImGui::Text("No entity selected");
			}

			ImGui::End();
		}
	}
	else
	{
		ImGui::Text("No CurrentWorld for EntityBrowser");
	}
}

void ImGuiEntityBrowser::DisplayEntity(Entity& entity, World* world)
{
	ImGui::PushID(entity.GetID());
	
	// Name
	const char* entityName = entity.GetName();
	if (entityName == nullptr || strlen(entityName) == 0)
	{
		entityName = kUnknownEntityName;
	}

	if (ImGui::Button(entityName))
	{
		if (!Keyboard::IsControlHold())
		{
			world->ClearSelectedEntities();
		}
		world->SelectEntity(entity);
	}

	if (entity.Has<TransformComponent>())
	{
		TransformComponent& transform = entity.Get<TransformComponent>();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("ENTITY_DRAG_N_DROP", &entity, sizeof(Entity));
			ImGui::Text("%s", entityName);
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG_N_DROP"))
			{
				enAssert(payload->DataSize == sizeof(Entity));
				transform.AttachChild(*(Entity*)payload->Data);
			}
			ImGui::EndDragDropTarget();
		}

		if (transform.GetChildrenCount() > 0)
		{
			ImGui::Indent();
			for (U32 i = 0; i < transform.GetChildrenCount(); ++i)
			{
				Entity child = transform.GetChild(i);
				if (child.IsValid())
				{
					DisplayEntity(child, world);
				}
			}
			ImGui::Unindent();
		}
	}

	ImGui::PopID();
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
