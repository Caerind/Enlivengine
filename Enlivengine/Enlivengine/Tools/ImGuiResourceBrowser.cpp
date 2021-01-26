#include <Enlivengine/Tools/ImGuiResourceBrowser.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <filesystem>

#include <imgui/imgui.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Resources/ResourceManager.hpp>
#include <Enlivengine/Resources/PathManager.hpp>

#include <Enlivengine/Graphics/Texture.hpp>
#include <Enlivengine/Audio/AudioManager.hpp>
#include <Enlivengine/Tiled/Tileset.hpp>
#include <Enlivengine/Tiled/Map.hpp>
#include <Enlivengine/Animation/Animation.hpp>
#include <Enlivengine/Animation/AnimationStateMachine.hpp>

#include <Enlivengine/Tools/ImGuiAnimationEditor.hpp>

#include <Enlivengine/Utils/XmlSerializer.hpp>

namespace en
{

ImGuiResourceBrowser::ImGuiResourceBrowser()
	: ImGuiTool()
{
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".png", Colors::Cyan.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_IMAGE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".jpg", Colors::Cyan.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_IMAGE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".dds", Colors::Cyan.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_IMAGE);

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".ttf", Colors::Orange.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_SIGNATURE);

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".tmx", Colors::Lime.WithAlpha(200).ToImGuiColor());
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".tsx", Colors::Peach.WithAlpha(200).ToImGuiColor());

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".ogg", Colors::BabyPink.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_AUDIO);

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".wav", Colors::HotPink.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_AUDIO);

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".astm", Colors::DarkYellow.WithAlpha(200).ToImGuiColor());

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".c", Colors::Yellow.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_CODE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".cpp", Colors::Yellow.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_CODE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".h", Colors::Yellow.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_CODE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".hpp", Colors::Yellow.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_CODE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".inl", Colors::Yellow.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_CODE);
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".sh", Colors::Yellow.WithAlpha(200).ToImGuiColor(), ICON_FA_FILE_CODE);

	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".json", Colors::LightGreen.WithAlpha(200).ToImGuiColor());
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".xml", Colors::LightBlue.WithAlpha(200).ToImGuiColor());
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".txt", Colors::Magenta.WithAlpha(200).ToImGuiColor());
	igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".md", Colors::Mint.WithAlpha(200).ToImGuiColor());
}

ImGuiToolTab ImGuiResourceBrowser::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiResourceBrowser::GetName() const
{
	return ICON_FA_COMPASS " ResourceBrowser";
}

const char* ImGuiResourceBrowser::GetSaveName() const
{
	return "ResourceBrowser";
}

void ImGuiResourceBrowser::Initialize()
{
	RegisterResourceSpecifics();

	LoadResourceInfosFromFile();
}

void ImGuiResourceBrowser::Display()
{
	AddNewResource();
	
	DisplayResources();
}

bool ImGuiResourceBrowser::LoadResourceInfosFromFile()
{
	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + "resources.data");
	if (std::filesystem::exists(path))
	{
		XmlSerializer xml;
		if (!xml.Open(path.string(), Serializer::Mode::Read))
		{
			return false;
		}

		Array<ResourceInfo> resourceInfos;
		if (GenericSerialization(xml, "Resources", mResourceInfos))
		{
			for (const ResourceInfo& resourceInfo : mResourceInfos)
			{
				const auto itr = mResourceSpecifics.find(resourceInfo.type);
				if (resourceInfo.loadInfo.IsFromFile())
				{
					if (itr != mResourceSpecifics.end())
					{
						if (!itr->second.loader(resourceInfo.identifier, resourceInfo.loadInfo.infoString))
						{
							enLogWarning(LogChannel::Core, "Can't load {}-{}", resourceInfo.identifier, resourceInfo.loadInfo.infoString);
						}
					}
					else
					{
						enLogError(LogChannel::Core, "The resource type {} is not registered to the ResourceBrowser", resourceInfo.type);
					}
				}
			}
		}

		return true;
	}
	else
	{
		return SaveResourceInfosToFile();
	}
}

bool ImGuiResourceBrowser::SaveResourceInfosToFile()
{
	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + "resources.data");

	XmlSerializer xml;
	if (xml.Open(path.string(), Serializer::Mode::Write))
	{
		GenericSerialization(xml, "Resources", mResourceInfos);
		return xml.Close();
	}
	else
	{
		enLogError(LogChannel::Tools, "Can't save Worlds");
		return false;
	}
}

void ImGuiResourceBrowser::RegisterResourceSpecific(U32 resourceType, ResourceSpecificFileLoader loader, ResourceSpecificPreview preview)
{
	auto& instance = GetInstance();
	instance.mResourceSpecifics[resourceType].name = std::string(ResourceManager::GetInstance().GetResourceTypeName(resourceType));
	instance.mResourceSpecifics[resourceType].loader = loader;
	instance.mResourceSpecifics[resourceType].preview = preview;
}

void ImGuiResourceBrowser::AddNewResource()
{
	static const std::string key = "ResourceBrowserFileDialogKey";
	static const char* dialogTitle = "ResourceBrowser: Choose File";

	const std::filesystem::path assetsPath = PathManager::GetAbsolutePath(PathManager::GetAssetsPath());

	ImGui::Text("Add New Resource");
	ImGui::Indent();

	const std::string currentLabel = std::string(ResourceManager::GetInstance().GetResourceTypeName(mResourceType));
	if (ImGui::BeginCombo("Type", currentLabel.c_str()))
	{
		const U32 resourceTypeCount = ResourceManager::GetInstance().GetResourceTypeCount();
		for (U32 i = 1; i < resourceTypeCount; ++i)
		{
			const std::string label = std::string(ResourceManager::GetInstance().GetResourceTypeName(i));
			bool selected = (i == mResourceType);
			if (ImGui::Selectable(label.c_str(), selected))
			{
				mResourceType = i;
				selected = true;
			}
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::InputText("Identitifer", mIdentifierBuffer, kBufferSize);
	if (ImGui::Button("..."))
	{
		igfd::ImGuiFileDialog::Instance()->OpenDialog(key, dialogTitle, ".*", assetsPath.string());
	}
	ImGui::SameLine();
	ImGui::InputText("Filename", mFilenameBuffer, kBufferSize);
	if (igfd::ImGuiFileDialog::Instance()->FileDialog(key))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk)
		{
			std::filesystem::path filename = igfd::ImGuiFileDialog::Instance()->GetFirstSelected();
			std::string relativeResult = filename.lexically_relative(assetsPath).string();
			std::replace(relativeResult.begin(), relativeResult.end(), '\\', '/');

#ifdef ENLIVE_COMPILER_MSVC
			strcpy_s(mFilenameBuffer, relativeResult.c_str());
#else
			strcpy(mFilenameBuffer, relativeResult.c_str());
#endif // ENLIVE_COMPILER_MSVC
		}
		igfd::ImGuiFileDialog::Instance()->CloseDialog(key);
	}
	if (mResourceType != 0 && strlen(mIdentifierBuffer) > 0 && strlen(mFilenameBuffer) > 0)
	{
		const auto itr = mResourceSpecifics.find(mResourceType);
		const bool registeredResourceType = itr != mResourceSpecifics.end();
		const bool resourceTypeCompatibleExtension = true; // TODO : Check that ResourceType is compatible with the extension of the file

		if (registeredResourceType && resourceTypeCompatibleExtension)
		{
			if (ImGui::Button("Add"))
			{
				std::string resourceIdentifier(mIdentifierBuffer);
				std::string resourceFilename(PathManager::GetAssetsPath() + mFilenameBuffer);
				itr->second.loader(resourceIdentifier, resourceFilename);

				ResourceManager::GetInstance().GetResourceInfos(mResourceInfos);
				SaveResourceInfosToFile();

#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(mIdentifierBuffer, "");
				strcpy_s(mFilenameBuffer, "");
#else
				strcpy(mIdentifierBuffer, "");
				strcpy(mFilenameBuffer, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}
		else
		{
			ImGui::DisabledButton("Add");
			if (ImGui::IsItemHovered())
			{
				if (!registeredResourceType)
				{
					ImGui::SetTooltip("This resource type is not registered to the ResourceBrowser");
				}
				else if (!resourceTypeCompatibleExtension)
				{
					ImGui::SetTooltip("Type and extension are not compatible");
				}
			}
		}
	}
	else
	{
		ImGui::DisabledButton("Add");
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Invalid input");
		}
	}
	ImGui::Unindent();
}

void ImGuiResourceBrowser::DisplayResources()
{
	ResourceManager::GetInstance().GetResourceInfos(mResourceInfos);

	bool deletedSome = false;
	U32 size = static_cast<U32>(mResourceInfos.Size());
	if (size > 0)
	{
		ImGui::Separator();

		ImGui::BeginChild("", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		U32 previousResourceType = mResourceInfos[0].type;
		std::string typeString = std::string(ResourceManager::GetInstance().GetResourceTypeName(previousResourceType));
		typeString += "s";
		bool currentResourceTypeOpened = ImGui::CollapsingHeader(typeString.c_str());
		if (currentResourceTypeOpened)
		{
			ImGui::Indent();
		}

		for (U32 i = 0; i < size; )
		{
			ResourceInfo& resourceInfo = mResourceInfos[i];
			if (resourceInfo.type > previousResourceType)
			{
				if (currentResourceTypeOpened)
				{
					ImGui::Unindent();
				}
				previousResourceType = resourceInfo.type;
				typeString = std::string(ResourceManager::GetInstance().GetResourceTypeName(previousResourceType));
				typeString += "s";
				currentResourceTypeOpened = ImGui::CollapsingHeader(typeString.c_str());
				if (currentResourceTypeOpened)
				{
					ImGui::Indent();
				}
			}

			if (currentResourceTypeOpened)
			{
				ImGui::PushID(i);

				if (resourceInfo.type == static_cast<U32>(ResourceType::Invalid))
				{
					ImGui::Text(ICON_FA_EXCLAMATION_TRIANGLE);
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(4, 0));
					ImGui::SameLine();
				}

				if (!resourceInfo.loaded && resourceInfo.type != static_cast<U32>(ResourceType::Music)) // TODO : Remove special case for Music
				{
					ImGui::Text(ICON_FA_EXCLAMATION);
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("This resource is not loaded");
						//ImGui::SetTooltip("This resource is not loaded yet. Click to load");
						/*
						if (ImGui::IsItemClicked())
						{
							// TODO : Reload ?
						}
						*/
					}

					ImGui::SameLine();
				}

				//ImVec4 color = ResourceInfo::ResourceInfoTypeToColor(resourceInfo.type).toImGuiColor();
				//ImGui::TextColored(color, "%s", resourceInfo.identifier.c_str());
				ImGui::Text("%s", resourceInfo.identifier.c_str());

				if (ImGui::IsItemHovered())
				{
					const std::string loadMethodString = std::string(Enum::GetValueName(resourceInfo.loadInfo.method));
					if (resourceInfo.loadInfo.method == ResourceLoadInfo::File || resourceInfo.loadInfo.method == ResourceLoadInfo::Download)
					{
						ImGui::SetTooltip("ID: %d\n%s (%s)", resourceInfo.id, loadMethodString.c_str(), resourceInfo.loadInfo.infoString.c_str());
					}
					else
					{
						ImGui::SetTooltip("ID: %d\n%s", resourceInfo.id, loadMethodString.c_str());
					}
				}
				ImGui::SameLine();

				if (resourceInfo.loaded || resourceInfo.type == static_cast<U32>(ResourceType::Music)) // TODO : Remove special case for Music
				{
					const auto itr = mResourceSpecifics.find(resourceInfo.type);
					if (itr != mResourceSpecifics.end())
					{
						itr->second.preview(resourceInfo);
					}
				}

				if (ImGui::Button("Remove"))
				{
					if (resourceInfo.id != InvalidResourceID)
					{
						deletedSome = true;

						// TODO : Remove special case for Sound
						if (resourceInfo.type == static_cast<U32>(ResourceType::Sound))
						{
							AudioManager::GetInstance().ReleaseSound(resourceInfo.id);
						}
						else
						{
							ResourceManager::GetInstance().Release(resourceInfo.id, resourceInfo.type);
						}
					}

					mResourceInfos.RemoveAtIndex(i);
					size--;
				}
				else
				{
					i++;
				}

				ImGui::PopID();
			}
			else
			{
				i++;
			}
		}

		if (currentResourceTypeOpened)
		{
			ImGui::Unindent();
		}

		ImGui::EndChild();
	}

	if (deletedSome)
	{
		SaveResourceInfosToFile();
	}
}

void ImGuiResourceBrowser::RegisterResourceSpecifics()
{
	// Image
	// TODO : Image Preview

	// Texture
	RegisterResourceSpecific<Texture>(
		[](const std::string& identifier, const std::string& filename)
		{
			return ResourceManager::GetInstance().Create(identifier, TextureLoader::FromFile(filename)).IsValid();
		},
		[](const ResourceInfo& resourceInfo)
		{
			ImGui::Text(ICON_FA_SEARCH);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				const Texture& texture = ResourceManager::GetInstance().Get<Texture>(resourceInfo.id).Get();
				ImGui::Image(texture.GetHandle(), ImVec2(100.0f, 100.0f));

				ImGui::EndTooltip();
			}
			ImGui::SameLine();
		}
		);

	// Font
	// TODO : Font Preview

	// Music
	// TODO : Music Preview

	// Sound
	// TODO : Sound Preview

	// Tileset
	// TODO : Tileset Preview

	// Map
	// TODO : Map Preview

	// Animation
	// TODO : Animation Preview

	// AnimationStateMachine
	// TODO : AnimationStateMachine Preview

	// Shader
	// TODO : Shader Preview
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
