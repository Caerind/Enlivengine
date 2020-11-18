#include <Enlivengine/Tools/ImGuiGame.hpp>

#if defined(ENLIVE_MODULE_TOOLS) && defined(ENLIVE_MODULE_CORE)
#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

ImGuiGame::ImGuiGame()
	: ImGuiTool()
	, mFramebuffer(BGFX_INVALID_HANDLE)
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

	if (bgfx::isValid(mFramebuffer))
	{
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImGui::Image(bgfx::getTexture(mFramebuffer), windowSize);
	}
}

void ImGuiGame::SetFramebuffer(bgfx::FrameBufferHandle framebuffer)
{
	GetInstance().mFramebuffer = framebuffer;
}

bgfx::FrameBufferHandle ImGuiGame::GetFramebuffer()
{
	return GetInstance().mFramebuffer;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS && ENLIVE_MODULE_CORE