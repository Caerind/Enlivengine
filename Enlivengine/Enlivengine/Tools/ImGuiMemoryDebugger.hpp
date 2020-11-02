#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_DEBUG_MEMORY) 

#include <Enlivengine/Tools/ImGuiToolManager.hpp>

namespace en
{

class ImGuiMemoryDebugger : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiMemoryDebugger);

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_DEBUG_MEMORY
#endif // ENLIVE_MODULE_TOOLS
