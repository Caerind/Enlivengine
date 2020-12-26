#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Resources/ResourceManager.hpp>

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::ResourcePtr)

template <typename T>
struct HasCustomSerialization<en::ResourcePtr<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::ResourcePtr<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::ResourcePtr<T>>();
			dataFile.Serialize_Common(static_cast<en::U32>(object.GetID()), "resourceID");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::ResourcePtr<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::ResourcePtr<T>>::GetHash());

			en::U32 resourceID;
			dataFile.Deserialize_Common(resourceID, "resourceID");

			object = en::ResourcePtr<T>(static_cast<en::ResourceID>(resourceID));

			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor<en::ResourcePtr<T>>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::ResourcePtr<T>& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			static en::Array<en::ResourceInfo> resourceInfos;
			en::ResourceManager::GetInstance().GetResourceInfosOfType<T>(resourceInfos);

			static const char* nullResourceIdentifier = "@Null";
			static const char* notLoadedResourceIdentifier = "@NotLoaded";
			const char* currentLabel = nullptr;

			if (object.HasValidID())
			{
				bool found = false;
				for (en::U32 i = 0; i < resourceInfos.Size() && !found; ++i)
				{
					if (resourceInfos[i].id == object.GetID())
					{
						currentLabel = resourceInfos[i].identifier.c_str();
						found = true;
					}
				}
				if (found)
				{
					ImGui::Text(ICON_FA_CHECK);
				}
				else
				{
					ImGui::Text(ICON_FA_EXCLAMATION);
					currentLabel = notLoadedResourceIdentifier;
				}
			}
			else
			{
				ImGui::Text(ICON_FA_EXCLAMATION);
				currentLabel = nullResourceIdentifier;
			}

			ImGui::SameLine();

			ImGui::PushID("ComboResourceIdentifier");
			if (ImGui::BeginCombo(name, currentLabel))
			{
				{
					const bool nullSelected = !object.HasValidID();
					if (ImGui::Selectable(nullResourceIdentifier, nullSelected))
					{
						object.Release();
						modified = true;
					}
					if (nullSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				for (en::U32 i = 0; i < resourceInfos.Size(); ++i)
				{
					bool selected = resourceInfos[i].id == object.GetID();
					if (ImGui::Selectable(resourceInfos[i].identifier.c_str(), selected))
					{
						object = en::ResourcePtr<T>(resourceInfos[i].id);
						modified = true;
						selected = true;
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopID();

			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI