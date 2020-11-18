#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <vector>

#include <Enlivengine/Utils/Enums.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiToolManager
{
	ENLIVE_SINGLETON(ImGuiToolManager);

public:
	void Initialize();
	void Release();

	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename);

	void Update();

private:
	void RegisterTools();
	void RegisterTool(ImGuiTool& tool);

	void ImGuiMain();
	void ImGuiTools();

private:
	std::vector<ImGuiTool*> mTools[Enum::GetCount<ImGuiToolTab>()];
	bool mRunning;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI