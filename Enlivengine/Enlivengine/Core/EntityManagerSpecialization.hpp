#pragma once

#include <Enlivengine/Core/CustomTraits.hpp>
#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/DataFile.hpp>
#include <Enlivengine/Core/ObjectEditor.hpp>

#include <Enlivengine/Core/EntitySpecialization.hpp>

template <>
struct CustomObjectEditor<en::EntityManager>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::EntityManager& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			if (ImGui::Button("New Entity"))
			{
				object.CreateEntity();
				modified = true;
			}
			object.Each([&object, &modified](auto entityEntt)
			{
				en::Entity entity(object, entityEntt);
				if (entity.IsValid())
				{
					ImGui::PushID(entity.GetID());
					if (ImGui::Button("x"))
					{
						object.DestroyEntity(entity);
						modified = true;
					}
					else
					{
						ImGui::SameLine();
						if (CustomObjectEditor<en::Entity>::ImGuiEditor(entity, entity.GetName()))
						{
							modified = true;
						}
					}
					ImGui::PopID();
				}
			});
		}
		return modified;
	}
};

template <>
struct CustomXmlSerialization<en::EntityManager>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::EntityManager& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			bool anyError = false;
			dataFile.WriteCurrentType<en::EntityManager>();
			object.Each([&dataFile, &object, &anyError](auto entityEntt)
			{
				en::Entity entity(const_cast<en::EntityManager&>(object), entityEntt);
				if (entity.IsValid())
				{
					if (!CustomXmlSerialization<en::Entity>::Serialize(dataFile, entity, entity.GetName()))
					{
						anyError = true;
					}
				}
			});
			parser.CloseNode();
			return !anyError;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::EntityManager& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			bool anyError = false;
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::EntityManager>::GetHash());
			if (parser.ReadFirstNode())
			{
				do
				{
					en::Entity entity = object.CreateEntity();
					if (entity.IsValid())
					{
						static const char* invalidEntityName = ""; // We already have the node open + we can have many entities with the same name
						if (!CustomXmlSerialization<en::Entity>::Deserialize(dataFile, entity, invalidEntityName))
						{
							anyError = true;
						}
					}
				} while (parser.NextSibling());
				parser.CloseNode();
			}
			parser.CloseNode();
			return !anyError;
		}
		else
		{
			return false;
		}
	}
};