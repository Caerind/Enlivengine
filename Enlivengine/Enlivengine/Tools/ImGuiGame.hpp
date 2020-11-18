#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_MODULE_TOOLS) && defined(ENLIVE_MODULE_CORE)
#ifdef ENLIVE_ENABLE_IMGUI

#include <bgfx/bgfx.h>

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiGame : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiGame);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;
	int GetWindowFlags() const override;

	void Initialize() override;
	void Release() override;

	void Display() override;

	static void SetFramebuffer(bgfx::FrameBufferHandle framebuffer);
	static bgfx::FrameBufferHandle GetFramebuffer();

private:
	bgfx::FrameBufferHandle mFramebuffer;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS
