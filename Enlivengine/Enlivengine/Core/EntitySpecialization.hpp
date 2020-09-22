#pragma once

#include <Enlivengine/Core/CustomTraits.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/DataFile.hpp>
#include <Enlivengine/Core/ObjectEditor.hpp>

#include <Enlivengine/Core/ComponentManager.hpp>

template <>
struct CustomObjectEditor<en::Entity>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Entity& object, const char* name)
	{
		const bool fromEntityBrowser = name == nullptr;
		bool display = true;

		if (!fromEntityBrowser)
		{
			display = ImGui::CollapsingHeader(name);
		}

		bool modified = false;
		if (display)
		{
			if (!fromEntityBrowser)
			{
				ImGui::Indent();
			}
			if (object.IsValid())
			{
				const en::U32 entityID = object.GetID();
				ImGui::Text("ID: %d", entityID);
				ImGui::PushID(entityID);

				const auto& componentInfos = en::ComponentManager::GetComponentInfos();
				static std::vector<en::U32> hasNot;
				hasNot.clear();
				const auto endItr = componentInfos.cend();
				for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
				{
					const auto& ci = itr->second;
					if (HasComponent(object, ci.enttID))
					{
						ImGui::PushID(ci.enttID);
						if (ImGui::Button("-"))
						{
							ci.remove(object);
							modified = true;
							ImGui::PopID();
							continue;
						}
						else
						{
							ImGui::SameLine();
							if (ci.editor(object))
							{
								modified = true;
							}
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
								ci.add(object);
								modified = true;
							}
							ImGui::PopID();
						}
						ImGui::EndPopup();
					}
				}

				ImGui::PopID();
			}
			else
			{
				ImGui::Text("Invalid entity");
			}
			if (!fromEntityBrowser)
			{
				ImGui::Unindent();
			}
		}
		return modified;
	}

private:
	using ComponentTypeID = ENTT_ID_TYPE;
	static bool HasComponent(const en::Entity& entity, ComponentTypeID enttComponentID)
	{
		// TODO : Factorize with CustomXmlSerialization<en::Entity>
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};

template <>
struct CustomXmlSerialization<en::Entity>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Entity& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Entity>();
			const auto& componentInfos = en::ComponentManager::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (HasComponent(object, ci.enttID))
				{
					ci.serialize(dataFile, object);
				}
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Entity& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (strcmp(name, "") == 0)
		{
			bool anyError = false;

			// We should already be on the node of the entity : See explanation on EntityManager::Deserialize
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Entity>::GetHash());
			if (parser.ReadFirstNode())
			{
				static std::vector<DeserializationComponentNode> componentNodes; // TODO : Move to Array
				componentNodes.clear();
				do
				{
					const std::string nodeName = parser.GetNodeName();
					const en::U32 nodeNameHash = en::Hash::SlowHash(nodeName);
					const en::U32 nodeType = dataFile.ReadCurrentType();
					const bool registeredComponent = en::ComponentManager::IsRegistered(nodeNameHash);
					if (nodeNameHash == nodeType && registeredComponent)
					{
						componentNodes.push_back({ nodeName, nodeNameHash });
					}
					else
					{
						if (nodeNameHash != nodeType)
						{
							enLogWarning(en::LogChannel::Core, "Incompatible component : {}({}) <-> {} ?", nodeNameHash, nodeName, nodeType);
						}
						if (!registeredComponent)
						{
							enLogWarning(en::LogChannel::Core, "Unregistered component : {}", nodeName);
						}
						anyError = true;
					}
				} while (parser.NextSibling());
				parser.CloseNode();

				// Now, we are back at the entity level, parse components
				for (const auto& componentNode : componentNodes)
				{
					const auto& componentInfos = en::ComponentManager::GetComponentInfos();
					enAssert(componentInfos.find(componentNode.hash) != componentInfos.end());
					if (!componentInfos.at(componentNode.hash).deserialize(dataFile, object))
					{
						anyError = true;
					}
				}
			}
			return !anyError;
		}
		else
		{
			// Single entity deserialization is not supported yet
			enAssert(false);
			return false;
		}
	}

private:
	struct DeserializationComponentNode
	{
		std::string name;
		en::U32 hash;
	};

	using ComponentTypeID = ENTT_ID_TYPE;
	static bool HasComponent(const en::Entity& entity, ComponentTypeID enttComponentID)
	{
		// TODO : Factorize with CustomObjectEditor<en::Entity>
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};
