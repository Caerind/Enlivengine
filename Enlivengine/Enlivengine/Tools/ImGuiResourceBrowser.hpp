#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiTool.hpp>
#include <Enlivengine/Resources/ResourceManager.hpp>

namespace en
{

class ImGuiResourceBrowser : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiResourceBrowser);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Display() override;

	bool LoadResourceInfosFromFile(const std::string& filename);
	bool SaveResourceInfosToFile(const std::string& filename);

	static U32 GetResourceTypeFromFilename(const std::string& filename);

private:
	void FontPreview(ResourceInfo& resourceInfo);
	void ImagePreview(ResourceInfo& resourceInfo);
	void TexturePreview(ResourceInfo& resourceInfo);
	void TilesetPreview(ResourceInfo& resourceInfo);
	void MapPreview(ResourceInfo& resourceInfo);
	void AnimationPreview(ResourceInfo& resourceInfo);
	void AnimationStateMachinePreview(ResourceInfo& resourceInfo);
	void MusicPreview(ResourceInfo& resourceInfo);
	void SoundPreview(ResourceInfo& resourceInfo);

private:
	// TODO : IMGUI UPDATE
	//static constexpr U32 kBufferSize{ 256 };
	//char mIdentifierBuffer[kBufferSize];
	//char mFilenameBuffer[kBufferSize];

	Array<ResourceInfo> mResourceInfos;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
