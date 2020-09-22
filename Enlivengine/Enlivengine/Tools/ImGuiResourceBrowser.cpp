#include <Enlivengine/Tools/ImGuiResourceBrowser.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#include <filesystem>

#include <Enlivengine/Application/PathManager.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/Math/Color.hpp>

#include <Enlivengine/Tools/ImGuiHelper.hpp>
#include <Enlivengine/Tools/ImGuiAnimationEditor.hpp>

#include <Enlivengine/Application/AudioSystem.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Map/Tileset.hpp>
#include <Enlivengine/Map/Map.hpp>
#include <Enlivengine/Graphics/Animation.hpp>
#include <Enlivengine/Graphics/AnimationStateMachine.hpp>

#include <SFML/Graphics/RenderTexture.hpp>

namespace en
{

ImGuiResourceBrowser::ImGuiResourceBrowser()
	: ImGuiTool()
{
	ImGuiFileDialog::Instance()->SetFilterColor(".png", Color::Cyan.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".jpg", Color::Cyan.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".ttf", Color::Orange.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".tmx", Color::Lime.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".tsx", Color::Peach.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".ogg", Color::BabyPink.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".wav", Color::HotPink.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".astm", Color::DarkYellow.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".cpp", Color::Yellow.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".h", Color::Yellow.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".hpp", Color::Yellow.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".inl", Color::Yellow.withAlpha(200).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".json", Color::LightGreen.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".xml", Color::LightBlue.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".txt", Color::Magenta.withAlpha(200).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".md", Color::Mint.withAlpha(200).toImGuiColor());
}

ImGuiToolTab ImGuiResourceBrowser::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiResourceBrowser::GetName() const
{
	return ICON_FA_COMPASS " ResourceBrowser";
}

void ImGuiResourceBrowser::Display()
{
	static const std::string key = "ResourceBrowserFileDialogKey";
	static const char* dialogTitle = "ResourceBrowser: Choose File";

	const std::filesystem::path assetsPath = PathManager::GetInstance().GetAssetsPathAbsolute();

	ImGui::Text("Add New Resource");
	ImGui::Indent(); 
	ImGui::InputText("Identitifer", mIdentifierBuffer, kBufferSize);
	if (ImGui::Button("..."))
	{
		ImGuiFileDialog::Instance()->OpenDialog(key, dialogTitle, nullptr, assetsPath.string());
	}
	ImGui::SameLine();
	ImGui::InputText("Filename", mFilenameBuffer, kBufferSize);
	if (ImGuiFileDialog::Instance()->FileDialog(key))
	{
		if (ImGuiFileDialog::Instance()->IsOk)
		{
			std::filesystem::path filepath = ImGuiFileDialog::Instance()->GetFilepathName();
			std::string relativeResult = filepath.lexically_relative(assetsPath).string();
			std::replace(relativeResult.begin(), relativeResult.end(), '\\', '/');

#ifdef ENLIVE_COMPILER_MSVC
			strcpy_s(mFilenameBuffer, relativeResult.c_str());
#else
			strcpy(mFilenameBuffer, relativeResult.c_str());
#endif // ENLIVE_COMPILER_MSVC
		}
		ImGuiFileDialog::Instance()->CloseDialog(key);
	}
	if (strlen(mIdentifierBuffer) > 0 && strlen(mFilenameBuffer) > 0)
	{
		if (ImGui::Button("Add"))
		{
			std::string resourceIdentifier(mIdentifierBuffer);
			std::string resourceFilename(mFilenameBuffer);
			ResourceID resourceID = ResourceManager::GetInstance().StringToResourceID(resourceIdentifier);
			U32 resourceType = GetResourceTypeFromFilename(resourceFilename);
			Application::GetInstance().LoadResource(resourceID, resourceType, PathManager::GetInstance().GetAssetsPath() + resourceFilename, resourceIdentifier);

			ResourceManager::GetInstance().GetResourceInfos(mResourceInfos);
			SaveResourceInfosToFile(PathManager::GetInstance().GetAssetsPath() + "resources.xml");

			AskForResize();

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
	}
	ImGui::Unindent();

	ResourceManager::GetInstance().GetResourceInfos(mResourceInfos);

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

				if (!resourceInfo.loaded && resourceInfo.type != static_cast<U32>(ResourceType::Music))
				{
					ImGui::Text(ICON_FA_EXCLAMATION);
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("This resource is not loaded");
						//ImGui::SetTooltip("This resource is not loaded yet. Click to load");
					}
					/*
					if (ImGui::IsItemClicked())
					{
						// TODO : Reload ?
					}
					*/
					ImGui::SameLine();
				}

				//ImVec4 color = ResourceInfo::ResourceInfoTypeToColor(resourceInfo.type).toImGuiColor();
				//ImGui::TextColored(color, "%s", resourceInfo.identifier.c_str());
				ImGui::Text("%s", resourceInfo.identifier.c_str());

				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("ID: %d", resourceInfo.id);
				}
				ImGui::SameLine();

				if (resourceInfo.loaded || resourceInfo.type == static_cast<U32>(ResourceType::Music))
				{
					switch (resourceInfo.type)
					{
						case static_cast<U32>(ResourceType::Font): FontPreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Image): ImagePreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Texture): TexturePreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Tileset): TilesetPreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Map): MapPreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Animation): AnimationPreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::AnimationStateMachine): AnimationStateMachinePreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Music): MusicPreview(resourceInfo); break;
						case static_cast<U32>(ResourceType::Sound): SoundPreview(resourceInfo); break;
						default: break;
					}
				}

				ImGui::Text(" : %s", resourceInfo.info.infoString.c_str());
				ImGui::SameLine();

				if (ImGui::Button("Remove"))
				{
					if (resourceInfo.id != InvalidResourceID)
					{
						if (resourceInfo.type == static_cast<U32>(ResourceType::Sound))
						{
							AudioSystem::GetInstance().ReleaseSound(resourceInfo.id);
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
}

bool ImGuiResourceBrowser::LoadResourceInfosFromFile(const std::string& filename)
{
	ParserXml xml;
	if (!xml.LoadFromFile(filename))
	{
		enLogError(en::LogChannel::Application, "Can't open resources file at {}", filename.c_str());
		return false;
	}

	if (xml.ReadNode("Resources"))
	{
		if (xml.ReadNode("Resource"))
		{
			do
			{
				U32 resourceIDu32;
				xml.GetAttribute("id", resourceIDu32);
				ResourceID resourceID = static_cast<ResourceID>(resourceIDu32);

				U32 resourceType;
				xml.GetAttribute("type", resourceType);

				std::string resourceIdentifer;
				bool resourceHasIdentifier = xml.GetAttribute("identifier", resourceIdentifer);

				std::string resourceFilename;
				bool resourceHasFilename = xml.GetAttribute("filename", resourceFilename);

				if (resourceHasFilename)
				{
					if (resourceHasIdentifier)
					{
						Application::GetInstance().LoadResource(resourceID, resourceType, PathManager::GetInstance().GetAssetsPath() + resourceFilename, resourceIdentifer);
					}
					else
					{
						enAssert(false); // Mandatory (for now)
					}
				}
				else
				{
					enAssert(false); // Mandatory (for now)
				}
				
			} while (xml.NextSibling("Resource"));
			xml.CloseNode();
		}
		xml.CloseNode();
	}
	else
	{
		enLogError(en::LogChannel::Application, "Invalid resources file at {}", filename.c_str());
		return false;
	}

	AskForResize();

	return true;
}

bool ImGuiResourceBrowser::SaveResourceInfosToFile(const std::string& filename)
{
	const std::string assetsPath = PathManager::GetInstance().GetAssetsPath();

	ParserXml xml;
	xml.NewFile();

	if (!xml.CreateNode("Resources"))
	{
		return false;
	}

	for (const ResourceInfo& resourceInfo : mResourceInfos)
	{
		if (!xml.CreateNode("Resource"))
		{
			continue;
		}

		xml.SetAttribute("identifier", resourceInfo.identifier);
		xml.SetAttribute("id", resourceInfo.id);
		xml.SetAttribute("type", resourceInfo.type);

		if (resourceInfo.IsFromFile())
		{
			std::string filepath = resourceInfo.GetFilename();
			filepath = filepath.substr(assetsPath.size());
			xml.SetAttribute("filename", filepath);
		}

		xml.CloseNode();
	}

	if (!xml.SaveToFile(filename))
	{
		return false;
	}

	return true;
}

U32 ImGuiResourceBrowser::GetResourceTypeFromFilename(const std::string& filename)
{
	std::string ext = std::filesystem::path(filename).extension().string();
	if (ext == ".ttf")
	{
		return static_cast<U32>(ResourceType::Font);
	}
	if (ext == ".bmp")
	{
		return static_cast<U32>(ResourceType::Image);
	}
	if (ext == ".png" || ext == ".jpg")
	{
		return static_cast<U32>(ResourceType::Texture);
	}
	if (ext == ".ogg")
	{
		return static_cast<U32>(ResourceType::Music);
	}
	if (ext == ".wav")
	{
		return static_cast<U32>(ResourceType::Sound);
	}
	if (ext == ".tsx")
	{
		return static_cast<U32>(ResourceType::Tileset);
	}
	if (ext == ".tmx")
	{
		return static_cast<U32>(ResourceType::Map);
	}
	if (ext == ".json")
	{
		return static_cast<U32>(ResourceType::Animation);
	}
	if (ext == ".astm")
	{
		return static_cast<U32>(ResourceType::AnimationStateMachine);
	}
	return static_cast<U32>(ResourceType::Invalid);
}

void ImGuiResourceBrowser::FontPreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Font));

	// TODO
}

void ImGuiResourceBrowser::ImagePreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Image));

	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		//const Image& image = ResourceManager::GetInstance().Get<Image>(resourceInfo.id).Get();
		//ImGui::PreviewTexture(image, 150.0f);

		ImGui::EndTooltip();
	}
	ImGui::SameLine();
}

void ImGuiResourceBrowser::TexturePreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Texture));

	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		const Texture& texture = ResourceManager::GetInstance().Get<Texture>(resourceInfo.id).Get();
		ImGui::PreviewTexture(texture, 150.0f);

		ImGui::EndTooltip();
	}
	ImGui::SameLine();
}

void ImGuiResourceBrowser::TilesetPreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Tileset));

	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		tmx::TilesetPtr tileset = ResourceManager::GetInstance().Get<tmx::Tileset>(resourceInfo.id);
		if (tileset.IsValid() && tileset.Get().GetTexture().IsValid())
		{
			ImGui::BeginTooltip();

			const Texture& texture = tileset.Get().GetTexture().Get();
			ImGui::PreviewTexture(texture, 150.0f);

			ImGui::EndTooltip();
		}

	}
	ImGui::SameLine();
}

void ImGuiResourceBrowser::MapPreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Map));

	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		tmx::MapPtr mapPtr = ResourceManager::GetInstance().Get<tmx::Map>(resourceInfo.id);
		if (mapPtr.IsValid())
		{
			ImGui::BeginTooltip();

			const tmx::Map& map = mapPtr.Get();

			const U32 sizeX = map.GetSize().x * map.GetTileSize().x;
			const U32 sizeY = map.GetSize().y * map.GetTileSize().y;
			sf::RenderTexture renderTexture;
			renderTexture.create(sizeX, sizeY);
			renderTexture.clear(sf::Color::Transparent);
			map.Render(renderTexture, true);
			renderTexture.display();

			constexpr F32 maxPreviewSize = 150.0f;
			sf::Sprite previewSprite;
			previewSprite.setTexture(renderTexture.getTexture());
			Vector2f textureSize;
			textureSize.x = static_cast<F32>(renderTexture.getSize().x);
			textureSize.y = static_cast<F32>(renderTexture.getSize().y);
			if (textureSize.x > maxPreviewSize || textureSize.y > maxPreviewSize)
			{
				const F32 larger = (textureSize.x > textureSize.y) ? textureSize.x : textureSize.y;
				const F32 scale = maxPreviewSize / larger;
				previewSprite.setScale(scale, scale);
			}
			ImGui::Image(previewSprite);

			ImGui::EndTooltip();
		}
	}
	ImGui::SameLine();
}

void ImGuiResourceBrowser::AnimationPreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Animation));

	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		AnimationPtr animation = ResourceManager::GetInstance().Get<Animation>(resourceInfo.id);
		if (animation.IsValid() && animation.Get().GetTexture().IsValid())
		{
			static ResourceID lastResourceID = 654321;
			static U32 animationClipIndex;
			static U32 animationClipFrameIndex;
			static Time animationAcc;
			if (lastResourceID != resourceInfo.id)
			{
				lastResourceID = resourceInfo.id;
				animationClipIndex = 0;
				animationClipFrameIndex = 0;
				animationAcc = Time::Zero();
			}

			ImGui::BeginTooltip();

            const Animation& anim = animation.Get();
			ImGui::PreviewAnimation(anim, 100.0f, animationClipIndex, animationClipFrameIndex, animationAcc);

			ImGui::EndTooltip();
		}
	}
	ImGui::SameLine();
}

void ImGuiResourceBrowser::AnimationStateMachinePreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::AnimationStateMachine));

	AnimationStateMachinePtr ptr = ResourceManager::GetInstance().Get<AnimationStateMachine>(resourceInfo.id);
	if (ptr.IsValid())
	{
		ImGui::Text(ICON_FA_DIRECTIONS);
		if (ImGui::IsItemHovered())
		{
			const AnimationPtr& animPtr = ptr.Get().GetAnimation();
			if (animPtr.IsValid() && animPtr.Get().GetTexture().IsValid())
			{
				static ResourceID lastResourceID = 654321;
				static U32 animationClipIndex;
				static U32 animationClipFrameIndex;
				static Time animationAcc;
				if (lastResourceID != resourceInfo.id)
				{
					lastResourceID = resourceInfo.id;
					animationClipIndex = 0;
					animationClipFrameIndex = 0;
					animationAcc = Time::Zero();
				}

				ImGui::BeginTooltip();

				const Animation& anim = animPtr.Get();
				ImGui::PreviewAnimation(anim, 100.0f, animationClipIndex, animationClipFrameIndex, animationAcc);

				ImGui::EndTooltip();
			}
		}
		if (ImGui::IsItemClicked())
		{
			ImGuiAnimationEditor::GetInstance().Initialize(ptr);
		}
		ImGui::SameLine();
	}
}

void ImGuiResourceBrowser::MusicPreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Music));

	static MusicPtr music;
	if (music.IsValid() && music.GetMusicID() == resourceInfo.id)
	{
		ImGui::Text(ICON_FA_STOP_CIRCLE);
		if (ImGui::IsItemClicked())
		{
			music.Stop();
			AudioSystem::GetInstance().PlayMusics();
		}
		ImGui::SameLine();
	}
	else
	{
		ImGui::Text(ICON_FA_PLAY_CIRCLE);
		if (ImGui::IsItemClicked())
		{
			AudioSystem::GetInstance().PauseMusics();
			music = AudioSystem::GetInstance().PlayMusic(resourceInfo.id, false);
		}
		ImGui::SameLine();
	}
}

void ImGuiResourceBrowser::SoundPreview(ResourceInfo& resourceInfo)
{
	enAssert(resourceInfo.type == static_cast<U32>(ResourceType::Sound));

	ImGui::Text(ICON_FA_PLAY_CIRCLE);
	if (ImGui::IsItemClicked())
	{
		AudioSystem::GetInstance().PlaySound(resourceInfo.id);
	}
	ImGui::SameLine();
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
