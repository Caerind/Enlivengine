#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiWorlds : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiWorlds);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Initialize() override;

	void Display() override;

private:
	void CurrentWorld();
	void AllWorlds();

	bool LoadWorldsFromFile();
	bool SaveWorldsToFile();

	std::vector<std::string> mWorlds;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI