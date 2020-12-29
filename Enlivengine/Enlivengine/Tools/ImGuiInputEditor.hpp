#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiInputEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiInputEditor);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Initialize() override;

    void Display() override;

private:
	bool Buttons();
	bool Axes();

	bool LoadInputsFromFile();
	bool SaveInputsToFile();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
