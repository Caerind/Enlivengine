#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <bgfx/bgfx.h>

#include <Enlivengine/Graphics/Camera.hpp>

#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiEditor);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;
	int GetWindowFlags() const override; 

	void Display() override;

	static void SetFramebuffer(bgfx::FrameBufferHandle framebuffer);
	static bgfx::FrameBufferHandle GetFramebuffer();

	static Camera& GetCamera();

	enum class GizmoOperation
	{
		Translate,
		Rotate,
		Scale
	};

private:
	bgfx::FrameBufferHandle mFramebuffer;
	Camera mCamera;
	bool mEditCamera;
	GizmoOperation mGizmoOperation;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)
