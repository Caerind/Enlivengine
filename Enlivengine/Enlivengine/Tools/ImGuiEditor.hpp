#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_MODULE_TOOLS) && defined(ENLIVE_MODULE_CORE)
#ifdef ENLIVE_ENABLE_IMGUI

#include <bgfx/bgfx.h>

#include <Enlivengine/Graphics/Camera.hpp>

#include <Enlivengine/Tools/ImGuiToolManager.hpp>

namespace en
{

class ImGuiEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiEditor);
	~ImGuiEditor();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;
	virtual int GetWindowFlags() const;

	virtual void Display();

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

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS && ENLIVE_MODULE_CORE
