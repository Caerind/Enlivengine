#include <Enlivengine/Tools/ImGuiGame.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

ImGuiGame::ImGuiGame()
	: ImGuiTool()
	, mFramebuffer()
{
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
		if (World* world = Universe::GetInstance().GetCurrentWorld())
		{
			if (world->IsPlaying())
			{
				if (ImGui::SmallButton("Pause" /*ICON_FA_PAUSE*/)) // TODO : FONT AWESOME
				{
					world->Pause();
				}
			}
			else
			{
				if (ImGui::SmallButton("Play" /*ICON_FA_PLAY*/)) // TODO : FONT AWESOME
				{
					world->Play();
				}
			}
		}
		ImGui::EndMenuBar();
	}

	ImVec2 windowSize = ImGui::GetWindowSize();
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		windowSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
	}
	const Vector2u imWindowSize = Vector2u(static_cast<U32>(windowSize.x), static_cast<U32>(windowSize.y));
	if (imWindowSize != mFramebuffer.GetSize())
	{
		mFramebuffer.Resize(imWindowSize);
	}
	ImGui::Image(mFramebuffer.GetTexture(), windowSize);
}

Framebuffer* ImGuiGame::GetFramebuffer()
{
	return &GetInstance().mFramebuffer;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)