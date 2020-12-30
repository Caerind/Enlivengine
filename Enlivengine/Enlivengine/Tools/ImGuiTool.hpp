#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Graphics/ImGuiHelper.hpp>

namespace en
{

enum class ImGuiToolTab : U32
{
	Main = 0,
	Engine,
	Game
};

class ImGuiTool
{
public:
	ImGuiTool();

	virtual ImGuiToolTab GetTab() const = 0;
	virtual const char* GetName() const = 0;
	virtual const char* GetSaveName() const = 0;

	virtual void Initialize();
	virtual void Release();

	virtual void Display();

	virtual int GetWindowFlags() const;
	virtual bool IsImGuiDemoTool() const;

	bool IsVisible() const;

private:
	bool mRegistered;

protected:
	friend class ImGuiToolManager;
	bool mVisible;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI