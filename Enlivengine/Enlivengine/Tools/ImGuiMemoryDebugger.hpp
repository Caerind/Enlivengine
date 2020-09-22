#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_DEBUG_MEMORY) 

#include <Enlivengine/Application/ImGuiToolManager.hpp>

namespace en
{

class ImGuiMemoryDebugger : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiMemoryDebugger);
	~ImGuiMemoryDebugger();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_DEBUG_MEMORY
