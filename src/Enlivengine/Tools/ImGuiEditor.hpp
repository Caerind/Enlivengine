#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/Graphics/RenderTarget.hpp>

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

    void UpdateView();

	static RenderTarget& GetRenderTarget();
	static Vector2i GetMouseScreenCoordinates();
	static bool IsMouseInView();
	static bool IsViewVisible();

	static bool IsShowingManipulator();
	static bool IsShowingDebug();

	static bool IsUsingEditorView();
	static View* GetView();

	enum class GizmoOperation
	{
		Translate,
		Rotate,
		Scale
	};

	enum class GizmoMode
	{
		Local,
		Global
	};

	static bool IsPlaying();
	static bool IsPaused();
	static bool IsStopped();

	static bool StartGame();
	static bool StopGame();

private:
	RenderTarget mRenderTarget;
	Rectf mViewRect;
	bool mViewVisible;

	View mView;
	bool mUseMainView;
	bool mEditConfig;
	bool mShowManipulator;
	bool mShowDebug;
	GizmoOperation mGizmoOperation;
	GizmoMode mGizmoMode;

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
ENLIVE_DEFINE_ENUM_INFO(en::ImGuiEditor::GizmoMode)

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)
