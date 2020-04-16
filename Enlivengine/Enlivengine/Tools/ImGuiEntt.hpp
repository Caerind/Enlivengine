#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>

namespace en
{

class ImGuiEntt : public ImGuiTool
{
public:
	static ImGuiEntt& GetInstance();

	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

private:
	ImGuiEntt();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
