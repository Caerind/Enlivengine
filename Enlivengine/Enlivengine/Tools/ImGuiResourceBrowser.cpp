#include <Enlivengine/Tools/ImGuiResourceBrowser.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#include <filesystem>

#include <Enlivengine/Application/PathManager.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Graphics/LinearColor.hpp>
#include <Enlivengine/System/ParserXml.hpp>

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
	ImGuiFileDialog::Instance()->SetFilterColor(".ttf", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Font).withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".tmx", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Map).withAlpha(0.7f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".tsx", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Tileset).withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".ogg", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Music).withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".wav", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Sound).withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".png", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Texture).withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".jpg", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::Texture).withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".astm", ResourceInfo::ResourceInfoTypeToColor(ResourceInfo::Type::AnimationStateMachine).withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".cpp", LinearColor::Yellow.withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".h", LinearColor::Yellow.withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".hpp", LinearColor::Yellow.withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".inl", LinearColor::Yellow.withAlpha(0.8f).toImGuiColor());

	ImGuiFileDialog::Instance()->SetFilterColor(".json", LinearColor::LightGreen.withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".xml", LinearColor::LightBlue.withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".txt", LinearColor::Magenta.withAlpha(0.8f).toImGuiColor());
	ImGuiFileDialog::Instance()->SetFilterColor(".md", LinearColor::Mint.withAlpha(0.8f).toImGuiColor());
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

	static std::filesystem::path assetsPath = PathManager::GetInstance().GetAssetsPath();

	ImGui::Text("Add New Resource");
	ImGui::Indent(); 
	ImGui::InputText("Identitifer", mIdentifierBuffer, kBufferSize);
	if (ImGui::Button("..."))
	{
		if (!assetsPath.is_absolute())
		{
			assetsPath = std::filesystem::absolute(assetsPath);
		}
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
			mResourceInfos.push_back(ResourceInfo());
			ResourceInfo& resourceInfo = mResourceInfos.back();
			resourceInfo.identifier = mIdentifierBuffer;
			resourceInfo.filename = mFilenameBuffer;
			resourceInfo.type = ResourceInfo::DetectTypeFromFilename(resourceInfo.filename);

			Application::GetInstance().LoadResource(static_cast<I32>(resourceInfo.type), resourceInfo.identifier, resourceInfo.filename, resourceInfo.resourceID);
			
			std::sort(mResourceInfos.begin(), mResourceInfos.end(), [](const ResourceInfo& a, const ResourceInfo& b) { return a.type < b.type; });

			AskForResize();

			SaveResourceInfosToFile(PathManager::GetInstance().GetAssetsPath() + "resources.xml");

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

	U32 size = static_cast<U32>(mResourceInfos.size());
	if (size > 0)
	{
		ImGui::Separator();

		ImGui::BeginChild("", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		ResourceInfo::Type previousType = mResourceInfos[0].type;
		for (U32 i = 0; i < size; ++i)
		{
			ResourceInfo& resourceInfo = mResourceInfos[i];
			if (resourceInfo.type > previousType)
			{
				ImGui::Separator();
				previousType = resourceInfo.type;
			}

			bool resourceLoaded = false;

			ImGui::PushID(i);

			if (resourceInfo.type == ResourceInfo::Type::Unknown)
			{
				ImGui::Text(ICON_FA_EXCLAMATION_TRIANGLE);
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(4, 0));
				ImGui::SameLine();
			}
			else
			{
				resourceLoaded = ResourceManager::GetInstance().Has(resourceInfo.resourceID);
			}

			if (!resourceLoaded && resourceInfo.type != ResourceInfo::Type::Music)
			{
				ImGui::Text(ICON_FA_PLUS_CIRCLE);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("This resource is not loaded yet. Click to load");
				}
				if (ImGui::IsItemClicked())
				{
					Application::GetInstance().LoadResource(static_cast<I32>(resourceInfo.type), resourceInfo.identifier, resourceInfo.filename, resourceInfo.resourceID);
				}
				ImGui::SameLine();
			}

			ImVec4 color = ResourceInfo::ResourceInfoTypeToColor(resourceInfo.type).toImGuiColor();

			ImGui::TextColored(color, "%s", resourceInfo.identifier.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("ID: %d", resourceInfo.resourceID);
			}
			ImGui::SameLine();

			if (resourceLoaded || resourceInfo.type == ResourceInfo::Type::Music)
			{
				switch (resourceInfo.type)
				{
				case ResourceInfo::Type::Font: FontPreview(resourceInfo); break;
				case ResourceInfo::Type::Texture: TexturePreview(resourceInfo); break;
				case ResourceInfo::Type::Tileset: TilesetPreview(resourceInfo); break;
				case ResourceInfo::Type::Map: MapPreview(resourceInfo); break;
				case ResourceInfo::Type::Animation: AnimationPreview(resourceInfo); break;
				case ResourceInfo::Type::AnimationStateMachine: AnimationStateMachinePreview(resourceInfo); break;
				case ResourceInfo::Type::Music: MusicPreview(resourceInfo); break;
				case ResourceInfo::Type::Sound: SoundPreview(resourceInfo); break;
				default: break;
				}
			}

			ImGui::Text(" : %s", resourceInfo.filename.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Remove"))
			{
				if (resourceInfo.resourceID != InvalidResourceID)
				{
					if (resourceInfo.type == ResourceInfo::Type::Sound)
					{
						AudioSystem::GetInstance().ReleaseSound(resourceInfo.resourceID);
					}
					else
					{
						ResourceManager::GetInstance().Release(resourceInfo.resourceID);
					}
				}

				mResourceInfos.erase(mResourceInfos.begin() + i);
				size--;
				i--;
			}

			ImGui::PopID();
		}
		ImGui::EndChild();
	}
}

bool ImGuiResourceBrowser::LoadResourceInfosFromFile(const std::string& filename)
{
	ParserXml xml;
	if (!xml.loadFromFile(filename))
	{
		LogError(en::LogChannel::Application, 9, "Can't open resources file at %s", filename.c_str());
		return false;
	}

	if (xml.readNode("Resources"))
	{
		U32 resourceCount = 0;
		xml.getAttribute("resourceCount", resourceCount);
		U32 typeCount = 0;
		xml.getAttribute("typeCount", typeCount);
		const bool needToRecomputeTypesFromFilename = (typeCount == static_cast<U32>(ResourceInfo::Type::Count));
		if (resourceCount > 0)
		{
			mResourceInfos.reserve(resourceCount);
			if (xml.readNode("Resource"))
			{
				do
				{
					mResourceInfos.push_back(ResourceInfo());
					ResourceInfo& resourceInfo = mResourceInfos.back();
					
					xml.getAttribute("identifier", resourceInfo.identifier);
					xml.getAttribute("filename", resourceInfo.filename);
					I32 typeInt;
					xml.getAttribute("type", typeInt);
					if (!needToRecomputeTypesFromFilename)
					{
						resourceInfo.type = static_cast<ResourceInfo::Type>(typeInt);
					}
					else
					{
						resourceInfo.type = ResourceInfo::DetectTypeFromFilename(resourceInfo.filename);
					}

					Application::GetInstance().LoadResource(static_cast<I32>(resourceInfo.type), resourceInfo.identifier, resourceInfo.filename, resourceInfo.resourceID);
				
				} while (xml.nextSibling("Resource"));
				xml.closeNode();
			}
		}
		xml.closeNode();
	}
	else
	{
		LogError(en::LogChannel::Application, 9, "Invalid resources file at %s", filename.c_str());
		return false;
	}

	std::sort(mResourceInfos.begin(), mResourceInfos.end(), [](const ResourceInfo& a, const ResourceInfo& b) { return a.type < b.type; });

	AskForResize();

	return true;
}

bool ImGuiResourceBrowser::SaveResourceInfosToFile(const std::string& filename)
{
	ParserXml xml;
	xml.newFile();

	if (!xml.createChild("Resources"))
	{
		return false;
	}
	xml.setAttribute("resourceCount", static_cast<U32>(mResourceInfos.size()));
	xml.setAttribute("typeCount", static_cast<U32>(ResourceInfo::Type::Count));

	for (const ResourceInfo& resourceInfo : mResourceInfos)
	{
		if (!xml.createChild("Resource"))
		{
			continue;
		}

		xml.setAttribute("identifier", resourceInfo.identifier);
		xml.setAttribute("filename", resourceInfo.filename);
		xml.setAttribute("type", static_cast<I32>(resourceInfo.type));

		xml.closeNode();
	}

	if (!xml.saveToFile(filename))
	{
		return false;
	}

	return true;
}

ImGuiResourceBrowser::ResourceInfo::Type ImGuiResourceBrowser::ResourceInfo::DetectTypeFromFilename(const std::string& filename)
{
	std::string ext = std::filesystem::path(filename).extension().string();
	if (ext == ".ttf")
	{
		return Type::Font;
	}
	if (ext == ".png" || ext == ".jpg")
	{
		return Type::Texture;
	}
	if (ext == ".tsx")
	{
		return Type::Tileset;
	}
	if (ext == ".tmx")
	{
		return Type::Map;
	}
	if (ext == ".json")
	{
		return Type::Animation;
	}
	if (ext == ".astm")
	{
		return Type::AnimationStateMachine;
	}
	if (ext == ".ogg")
	{
		return Type::Music;
	}
	if (ext == ".wav")
	{
		return Type::Sound;
	}
	return Type::Unknown;
}

const char* ImGuiResourceBrowser::ResourceInfo::ResourceInfoTypeToString(Type type)
{
	switch (type)
	{
	case ResourceInfo::Type::Unknown: return "Unknown"; break;
	case ResourceInfo::Type::Font: return "Font"; break;
	case ResourceInfo::Type::Texture: return "Texture"; break;
	case ResourceInfo::Type::Tileset: return "Tileset"; break;
	case ResourceInfo::Type::Map: return "Map"; break;
	case ResourceInfo::Type::Animation: return "Animation"; break;
	case ResourceInfo::Type::AnimationStateMachine: return "AnimationStateMachine"; break;
	case ResourceInfo::Type::Music: return "Music"; break;
	case ResourceInfo::Type::Sound: return "Sound"; break;
	default: assert(false); break;
	}
	return "";
}

const LinearColor& ImGuiResourceBrowser::ResourceInfo::ResourceInfoTypeToColor(Type type)
{
	static const LinearColor& unknownColor = LinearColor::Red;
	static LinearColor resourceInfoTypeColors[static_cast<U32>(ResourceInfo::Type::Count)] = 
	{
		LinearColor::Orange, // Font
		LinearColor::Cyan, // Texture
		LinearColor::Peach, // Tileset
		LinearColor::Lime, // Map
		LinearColor::DarkGreen, // Animation
		LinearColor::DarkYellow, // AnimationStateMachine
		LinearColor::BabyPink, // Music
		LinearColor::HotPink, // Sound
	};
	if (type == ResourceInfo::Type::Unknown)
	{
		return unknownColor;
	}
	else
	{
		return resourceInfoTypeColors[static_cast<U32>(type)];
	}
}

void ImGuiResourceBrowser::FontPreview(ResourceInfo& resourceInfo)
{
	ENLIVE_UNUSED(resourceInfo);
}

void ImGuiResourceBrowser::TexturePreview(ResourceInfo& resourceInfo)
{
	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		const Texture& texture = ResourceManager::GetInstance().Get<Texture>(resourceInfo.resourceID).Get();
		ImGui::PreviewTexture(texture, 150.0f);

		ImGui::EndTooltip();
	}
	ImGui::SameLine();
}

void ImGuiResourceBrowser::TilesetPreview(ResourceInfo& resourceInfo)
{
	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		tmx::TilesetPtr tileset = ResourceManager::GetInstance().Get<tmx::Tileset>(resourceInfo.resourceID);
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
	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		tmx::MapPtr mapPtr = ResourceManager::GetInstance().Get<tmx::Map>(resourceInfo.resourceID);
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
	ImGui::Text(ICON_FA_SEARCH);
	if (ImGui::IsItemHovered())
	{
		AnimationPtr animation = ResourceManager::GetInstance().Get<Animation>(resourceInfo.resourceID);
		if (animation.IsValid() && animation.Get().GetTexture().IsValid())
		{
			static ResourceID lastResourceID = 654321;
			static U32 animationClipIndex;
			static U32 animationClipFrameIndex;
			static Time animationAcc;
			if (lastResourceID != resourceInfo.resourceID)
			{
				lastResourceID = resourceInfo.resourceID;
				animationClipIndex = 0;
				animationClipFrameIndex = 0;
				animationAcc = Time::Zero;
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
	AnimationStateMachinePtr ptr = ResourceManager::GetInstance().Get<AnimationStateMachine>(resourceInfo.resourceID);
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
				if (lastResourceID != resourceInfo.resourceID)
				{
					lastResourceID = resourceInfo.resourceID;
					animationClipIndex = 0;
					animationClipFrameIndex = 0;
					animationAcc = Time::Zero;
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
	static MusicPtr music;
	if (music.IsValid() && music.GetMusicID() == resourceInfo.resourceID)
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
			music = AudioSystem::GetInstance().PlayMusic(resourceInfo.resourceID, false);
		}
		ImGui::SameLine();
	}
}

void ImGuiResourceBrowser::SoundPreview(ResourceInfo& resourceInfo)
{
	ImGui::Text(ICON_FA_PLAY_CIRCLE);
	if (ImGui::IsItemClicked())
	{
		AudioSystem::GetInstance().PlaySound(resourceInfo.resourceID);
	}
	ImGui::SameLine();
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
