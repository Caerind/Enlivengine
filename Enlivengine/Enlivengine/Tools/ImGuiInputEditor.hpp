#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#if defined(ENLIVE_ENABLE_IMGUI)

#include <Enlivengine/Application/ImGuiToolManager.hpp>

#include <Enlivengine/Application/ActionSystem.hpp>

namespace en
{

class ImGuiInputEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiInputEditor);
	~ImGuiInputEditor();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

    virtual void Display();
	
private:
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
