#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <vector>

#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/Enums.hpp>
#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiToolManager
{
public:
	static bool Initialize();
	static bool IsInitialized();
	static bool Release();

	static bool LoadFromFile(const std::string& filename);
	static bool SaveToFile(const std::string& filename);

	static void Update(Window& window);

	static void RegisterTool(ImGuiTool& tool);

private:
	void RegisterTools();

	void ImGuiMain();
	void ImGuiTools();

private:
	static ImGuiToolManager& GetInstance();
	ImGuiToolManager();
	~ImGuiToolManager();

private:
	std::vector<ImGuiTool*> mTools[Enum::GetCount<ImGuiToolTab>()];
	bool mInitialized;
	bool mRunning;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI