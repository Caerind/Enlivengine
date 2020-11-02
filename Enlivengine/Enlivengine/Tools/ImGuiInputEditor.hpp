#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Tools/ImGuiToolManager.hpp>

namespace en
{

class ImGuiInputEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiInputEditor);

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

    virtual void Display();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
