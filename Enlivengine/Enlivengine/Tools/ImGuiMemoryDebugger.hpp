#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_DEBUG_MEMORY) 

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiMemoryDebugger : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiMemoryDebugger);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Display() override;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_DEBUG_MEMORY
