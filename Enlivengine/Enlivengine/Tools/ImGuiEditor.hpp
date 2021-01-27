#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <Enlivengine/Graphics/Camera.hpp>
#include <Enlivengine/Graphics/Framebuffer.hpp>

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

	void UpdateCamera();

	static Framebuffer& GetFramebuffer();
	static Vector2i GetMouseScreenCoordinates();
	static bool IsMouseInView();
	static bool IsViewVisible();

	static bool IsShowingManipulator();
	static bool IsShowingDebug();

	static bool IsUsingEditorCamera();
	static Camera& GetCamera();

	enum class GizmoOperation
	{
		Translate,
		Rotate,
		Scale
	};

	static bool IsPlaying();
	static bool IsPaused();
	static bool IsStopped();

	static bool StartGame();
	static bool StopGame();

private:
	Framebuffer mFramebuffer;
	Rectf mViewRect;
	bool mViewVisible;

	Camera mCamera;
	bool mUseMainCamera;
	bool mEditConfig;
	bool mShowManipulator;
	bool mShowDebug;
	GizmoOperation mGizmoOperation;

	enum class GameStatus
	{
		Stopped,
		Playing,
		Paused,
		OneFrame
	};
	GameStatus mStatus;
};

} // namespace en

ENLIVE_DEFINE_ENUM_INFO(en::ImGuiEditor::GizmoOperation)

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)
