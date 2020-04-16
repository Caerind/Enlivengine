#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>

namespace en
{

class ImGuiDemoWindow : public ImGuiTool
{
public:
	static ImGuiDemoWindow& GetInstance();

	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

	virtual bool IsImGuiDemoTool() const;

private:
	ImGuiDemoWindow();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
