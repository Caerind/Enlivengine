#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/UIDComponent.hpp>

#include <Enlivengine/Core/ComponentFactory.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#include <IconFontCppHeaders/IconsFontAwesome5.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

Entity::Entity()
	: mManager(nullptr)
	, mEntity(entt::null)
{
}

Entity::Entity(EntityManager& manager, entt::entity entity)
	: mManager(&manager)
	, mEntity(entity)
{
}

Entity::Entity(World& world, entt::entity entity)
	: mManager(&world.GetEntityManager())
	, mEntity(entity)
{
}

bool Entity::operator==(const Entity& other) const
{
	return mManager == other.mManager && mEntity == other.mEntity;
}

bool Entity::operator!=(const Entity& other) const
{
	return !operator==(other);
}

bool Entity::IsValid() const
{
	return mManager != nullptr && GetRegistry().valid(mEntity);
}

U32 Entity::GetID() const
{
	return static_cast<U32>(entt::to_integral(mEntity));
}

U32 Entity::GetIndex() const
{
	return static_cast<U32>(GetRegistry().entity(mEntity));
}

U32 Entity::GetVersion() const
{
	return static_cast<U32>(GetRegistry().version(mEntity));
}
  
U32 Entity::GetUID() const
{
	enAssert(Has<UIDComponent>());
	return static_cast<U32>(Get<UIDComponent>().GetUID());
}

void Entity::Destroy()
{
	enAssert(IsValid());
	mManager->DestroyEntity(*this);
}

const char* Entity::GetName() const
{
	if (!IsValid() || !Has<NameComponent>())
	{
		return "";
	}
	else
	{
		return Get<NameComponent>().name.c_str();
	}
}

Vector3f Entity::GetPosition() const
{
	if (!IsValid() || !Has<TransformComponent>())
	{
		return Vector3f();
	}
	else
	{
		return Get<TransformComponent>().GetPosition();
	}
}

bool Entity::HasManager() const
{
	return mManager != nullptr;
}

EntityManager& Entity::GetManager()
{
	enAssert(mManager != nullptr);
	return *mManager;
}

const EntityManager& Entity::GetManager() const
{
	enAssert(mManager != nullptr);
	return *mManager;
}

World& Entity::GetWorld()
{
	enAssert(mManager != nullptr);
	return mManager->GetWorld();
}

const World& Entity::GetWorld() const
{
	enAssert(mManager != nullptr);
	return mManager->GetWorld();
}

bool Entity::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Entity>::GetName(), TypeInfo<Entity>::GetHash()))
	{
		bool ret = true;

		if (serializer.IsReading())
		{
			const auto& componentInfos = ComponentFactory::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (serializer.HasNode(ci.name))
				{
					ci.add(*this);
					ret = ci.serialize(serializer, *this) && ret;
				}
			}
		}
		else if (serializer.IsWriting())
		{
			const auto& componentInfos = ComponentFactory::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (ci.has(*this))
				{
					ret = ci.serialize(serializer, *this) && ret;
				}
			}
		}
		else
		{
			enAssert(false);
			ret = false;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool Entity::Edit(ObjectEditor& objectEditor, const char* name)
{
#ifdef ENLIVE_ENABLE_IMGUI
	bool ret = false;
	if (objectEditor.IsImGuiEditor())
	{
		bool collasping = false;
		bool onlyEntitySelected = false;

		World* world = (mManager != nullptr) ? &(mManager->GetWorld()) : nullptr;
		if (world != nullptr && world->IsSelected(*this) && world->GetSelectedEntities().size() == 1)
		{
			collasping = true;
			onlyEntitySelected = true;
		}
		else
		{
			collasping = objectEditor.BeginClass(name, TypeInfo<Entity>::GetName(), TypeInfo<Entity>::GetHash());
		}

		if (collasping)
		{
			if (IsValid())
			{
				ImGui::Indent();
			}
		}

		if (collasping)
		{
			if (IsValid())
			{
				const U32 entityID = GetID();
				ImGui::Text("ID: %d, Index:%d, Version:%d", entityID, GetIndex(), GetVersion());
				ImGui::SameLine();
				bool destroyed = false;
				if (ImGui::SmallButton(ICON_FA_BAN))
				{
					destroyed = true;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Destroy");
				}

				ImGui::PushID(entityID);

				if (onlyEntitySelected)
				{
					ImGui::Indent();
				}

				const auto& componentInfos = ComponentFactory::GetComponentInfos();
				std::vector<U32> hasNot;
				const auto endItr = componentInfos.cend();
				for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
				{
					const auto& ci = itr->second;
					if (ci.has(*this))
					{
						ImGui::PushID(itr->first);
						if (ImGui::Button("-"))
						{
							ci.remove(*this);
							ret = true;
						}
						else
						{
							ImGui::SameLine();
							if (ci.editor(objectEditor, *this))
							{
								ret = true;
							}
						}
						ImGui::PopID();
					}
					else
					{
						hasNot.push_back(itr->first);
					}
				}

				if (!hasNot.empty())
				{
					if (ImGui::Button("+ Add Component"))
					{
						ImGui::OpenPopup("Add Component");
					}
					if (ImGui::BeginPopup("Add Component"))
					{
						ImGui::TextUnformatted("Available:");
						ImGui::Separator();
						for (auto componentHash : hasNot)
						{
							const auto& ci = componentInfos.at(componentHash);
							ImGui::PushID(componentHash);
							if (ImGui::Selectable(ci.name))
							{
								ci.add(*this);
								ret = true;
							}
							ImGui::PopID();
						}
						ImGui::EndPopup();
					}
				}
				
				if (onlyEntitySelected)
				{
					ImGui::Unindent();
				}

				ImGui::PopID();

				if (destroyed)
				{
					Destroy();
				}
			}
			else
			{
				ImGui::Text("Invalid entity");
			}
			objectEditor.EndClass();
			return ret;
		}
	}
#else
	ENLIVE_UNUSED(objectEditor);
	ENLIVE_UNUSED(name);
#endif // ENLIVE_ENABLE_IMGUI
	return false;
}

const entt::entity& Entity::GetEntity() const
{
	return mEntity;
}

entt::registry& Entity::GetRegistry()
{
	enAssert(mManager != nullptr);
	return mManager->GetRegistry();
}

const entt::registry& Entity::GetRegistry() const
{
	enAssert(mManager != nullptr);
	return mManager->GetRegistry();
}

} // namespace en