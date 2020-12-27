#include <Enlivengine/Tools/ImGuiGame.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <Enlivengine/Core/Engine.hpp>
#include <Enlivengine/Core/World.hpp>

#include <Enlivengine/Window/Mouse.hpp>

namespace en
{

ImGuiGame::ImGuiGame()
	: ImGuiTool()
	, mFramebuffer()
	, mViewRect()
	, mViewVisible(false)
{
	mFramebuffer.Create(Vector2u(840, 600), true);
}

ImGuiToolTab ImGuiGame::GetTab() const
{
	return ImGuiToolTab::Game;
}

const char* ImGuiGame::GetName() const
{
	return "Game";
}

const char* ImGuiGame::GetSaveName() const
{
	return "Game";
}

int ImGuiGame::GetWindowFlags() const
{
	return ImGuiWindowFlags_MenuBar;
}

void ImGuiGame::Initialize()
{
}

void ImGuiGame::Release()
{
}

void ImGuiGame::Display()
{
	if (ImGui::BeginMenuBar())
	{
		if (World* world = Engine::GetCurrentWorld())
		{
			if (world->IsPlaying())
			{
				if (ImGui::SmallButton(ICON_FA_PAUSE))
				{
					world->Pause();
				}
			}
			else
			{
				if (ImGui::SmallButton(ICON_FA_PLAY))
				{
					world->Play();
				}
			}
		}
		ImGui::EndMenuBar();
	}

	const ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	const ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	const ImVec2 windowPos = ImGui::GetWindowPos();
	mViewRect.SetMin(Vector2f(vMin.x + windowPos.x, vMin.y + windowPos.y));
	mViewRect.SetMax(Vector2f(vMax.x + windowPos.x, vMax.y + windowPos.y));
	const Vector2f windowSize = mViewRect.GetSize();

	const Vector2u uWindowSize = Vector2u(windowSize);
	if (uWindowSize != mFramebuffer.GetSize() && uWindowSize.x > 0 && uWindowSize.y > 0)
	{
		mFramebuffer.Resize(uWindowSize);
	}
	if (uWindowSize.x > 0 && uWindowSize.y > 0)
	{
		ImGui::Image(mFramebuffer.GetTexture(), ImVec2(windowSize.x, windowSize.y));

		mViewVisible = ImGui::IsItemVisible(); // TODO : Not really working...
	}
	else
	{
		mViewVisible = false;
	}
}

Framebuffer* ImGuiGame::GetFramebuffer()
{
	return &GetInstance().mFramebuffer;
}

Vector2i ImGuiGame::GetMouseScreenCoordinates()
{
	return Mouse::GetPositionCurrentWindow() - Vector2i(GetInstance().mViewRect.GetMin());
}

bool ImGuiGame::IsMouseInView()
{
	return IsViewVisible() && GetInstance().mViewRect.Contains(Vector2f(Mouse::GetPositionCurrentWindow()));
}

bool ImGuiGame::IsViewVisible()
{
	return GetInstance().IsVisible() && GetInstance().mViewVisible;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)