#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>

namespace en
{

class ImGuiPhysic : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiPhysic);
	~ImGuiPhysic();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
