#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <Enlivengine/Graphics/Framebuffer.hpp>

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

	static Framebuffer* GetFramebuffer();
	static Vector2i GetMouseScreenCoordinates();
	static bool IsMouseInView();
	static bool IsViewVisible();

private:
	Framebuffer mFramebuffer;
	Rectf mViewRect;
	bool mViewVisible;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)
