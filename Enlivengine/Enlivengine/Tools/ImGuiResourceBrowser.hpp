#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>

namespace en
{

class ImGuiResourceBrowser : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiResourceBrowser);

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

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
	static constexpr U32 kBufferSize{ 256 };
	char mIdentifierBuffer[kBufferSize];
	char mFilenameBuffer[kBufferSize];

	Array<ResourceInfo> mResourceInfos;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
