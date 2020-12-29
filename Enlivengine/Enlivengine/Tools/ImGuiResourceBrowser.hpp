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

	void Initialize() override;

	void Display() override;

	using ResourceSpecificFileLoader = std::function<bool(const std::string& str, const std::string& filename)>;
	using ResourceSpecificPreview = std::function<void(const ResourceInfo&)>;
	template <typename T>
	static void RegisterResourceSpecific(ResourceSpecificFileLoader loader, ResourceSpecificPreview preview);
	static void RegisterResourceSpecific(U32 resourceType, ResourceSpecificFileLoader loader, ResourceSpecificPreview preview);

private:
	void AddNewResource();
	void DisplayResources();

	bool LoadResourceInfosFromFile();
	bool SaveResourceInfosToFile();

	void RegisterResourceSpecifics();

private:
	U32 mResourceType;
	static constexpr U32 kBufferSize{ 256 };
	char mIdentifierBuffer[kBufferSize];
	char mFilenameBuffer[kBufferSize];

	Array<ResourceInfo> mResourceInfos;

	struct ResourceSpecific
	{
		std::string name;
		ResourceSpecificFileLoader loader;
		ResourceSpecificPreview preview;
	};
	std::unordered_map<U32, ResourceSpecific> mResourceSpecifics;
};

template <typename T>
void ImGuiResourceBrowser::RegisterResourceSpecific(ResourceSpecificFileLoader loader, ResourceSpecificPreview preview)
{
	RegisterResourceSpecific(T::GetStaticResourceType(), loader, preview);
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
