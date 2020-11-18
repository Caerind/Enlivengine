#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiDemoWindow : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiDemoWindow);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Display() override;

	bool IsImGuiDemoTool() const override;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
