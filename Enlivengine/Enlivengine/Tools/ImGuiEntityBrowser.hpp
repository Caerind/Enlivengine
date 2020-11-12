#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI
#ifdef ENLIVE_MODULE_META
#ifdef ENLIVE_MODULE_CORE

#include <Enlivengine/Tools/ImGuiToolManager.hpp>

namespace en
{

class ImGuiEntityBrowser : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiEntityBrowser);
	~ImGuiEntityBrowser();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();
};

} // namespace en

#endif // ENLIVE_MODULE_CORE
#endif // ENLIVE_MODULE_META
#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
