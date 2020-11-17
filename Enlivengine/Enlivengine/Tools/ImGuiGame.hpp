#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_MODULE_TOOLS) && defined(ENLIVE_MODULE_CORE)
#ifdef ENLIVE_ENABLE_IMGUI

#include <bgfx/bgfx.h>

#include <Enlivengine/Tools/ImGuiToolManager.hpp>

namespace en
{

class ImGuiGame : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiGame);
	~ImGuiGame();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;
	virtual int GetWindowFlags() const;

	virtual void Display();

	static void SetFramebuffer(bgfx::FrameBufferHandle framebuffer);
	static bgfx::FrameBufferHandle GetFramebuffer();

private:
	bgfx::FrameBufferHandle mFramebuffer;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
