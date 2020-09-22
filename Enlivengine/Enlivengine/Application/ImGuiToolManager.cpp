#include <Enlivengine/Application/ImGuiToolManager.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <filesystem>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/Profiler.hpp>
#include <Enlivengine/Application/PathManager.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

ImGuiTool::ImGuiTool()
	: mRegistered(false)
	, mVisible(false)
{
}

void ImGuiTool::Display()
{
}

void ImGuiTool::Register()
{
	enAssert(!mRegistered);
	ImGuiToolManager::GetInstance().RegisterTool(this);
	mRegistered = true;
}

void ImGuiTool::Unregister()
{
	enAssert(mRegistered);
	ImGuiToolManager::GetInstance().UnregisterTool(this);
	mRegistered = false;
}

bool ImGuiTool::IsRegistered() const
{
	return mRegistered;
}

int ImGuiTool::GetWindowFlags() const
{
	return ImGuiWindowFlags_None;
}

bool ImGuiTool::IsImGuiDemoTool() const
{
	return false;
}

void ImGuiTool::AskForResize()
{
	mShouldResize = true;
}

void ImGuiTool::AskForFocus()
{
	mShouldFocus = true;
}

bool ImGuiTool::ShouldResize() const
{
	return mShouldResize;
}

void ImGuiTool::Resize()
{
	ImGui::SetWindowSize(ImVec2(0.0f, 0.0f));
	mShouldResize = false;
}

bool ImGuiTool::ShouldFocus() const
{
	return mShouldFocus;
}

void ImGuiTool::Focus()
{
	ImGui::SetWindowFocus();
	mShouldFocus = false;
}

ImGuiToolManager::ImGuiToolManager()
	: mShowImGui(false)
	, mRunning(false)
{
}

void ImGuiToolManager::RegisterTool(ImGuiTool* tool)
{
	enAssert(tool != nullptr);

	const U32 tab = static_cast<U32>(tool->GetTab());
	enAssert(tab >= static_cast<U32>(ImGuiToolTab::Main) && tab < static_cast<U32>(ImGuiToolTab::Count));

	std::vector<ImGuiTool*>& tools = mTools[tab];

	const U32 toolHash = Hash::SlowHash(tool->GetName());
	const size_t size = tools.size();
	for (size_t i = 0; i < size; ++i)
	{
		enAssert(Hash::SlowHash(tools[i]->GetName()) != toolHash);
	}

	tools.push_back(tool);
}

void ImGuiToolManager::UnregisterTool(ImGuiTool* tool)
{
	enAssert(tool != nullptr);

    const U32 tab = static_cast<U32>(tool->GetTab());
	enAssert(tab >= static_cast<U32>(ImGuiToolTab::Main) && tab < static_cast<U32>(ImGuiToolTab::Count));

	std::vector<ImGuiTool*>& tools = mTools[tab];

	const U32 toolHash = Hash::SlowHash(tool->GetName());
	const size_t size = tools.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (Hash::SlowHash(tools[i]->GetName()) == toolHash)
		{
			tools.erase(tools.begin() + i);
			return;
		}
	}

	enAssert(false);
}

void ImGuiToolManager::Initialize(Window& window)
{
	ImGui::SFML::Init(window.getHandle());

	if (kImGuiColorsClassic)
	{
		ImGui::StyleColorsClassic();
	}

	ImGuiIO& io = ImGui::GetIO(); 
	io.Fonts->Clear();
    io.Fonts->AddFontDefault();

	const std::string fontPath = en::PathManager::GetInstance().GetFontsPath() + std::string(FONT_ICON_FILE_NAME_FAS);
	if (std::filesystem::exists(std::filesystem::path(fontPath)))
	{
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f, &icons_config, icons_ranges);
	}

	ImGui::SFML::UpdateFontTexture();

#ifdef ENLIVE_DEBUG
	mShowImGui = true;
#else
	mShowImGui = false;
#endif // ENLIVE_DEBUG
	mRunning = true;
}

void ImGuiToolManager::Shutdown()
{
	if (mRunning)
	{
		ImGui::SFML::Shutdown();
		mRunning = false;
	}
}

void ImGuiToolManager::Update(Window& window, Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	if (mRunning)
	{
		ImGui::SFML::Update(window.getHandle(), toSF(dt));
		if (mShowImGui)
		{
			ImGuiMain();
		}
	}
}

void ImGuiToolManager::Render()
{
	ENLIVE_PROFILE_FUNCTION();
	if (mRunning)
	{
		ImGui::Render();
	}
}

void ImGuiToolManager::RenderFrame(Window& window)
{
	ENLIVE_PROFILE_FUNCTION();
	if (mRunning && mShowImGui)
	{
		auto& windowHandle = window.getHandle();
		ImGui::SFML::Render(windowHandle);
		windowHandle.resetGLStates();
	}
}

void ImGuiToolManager::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Menu)
	{
		mShowImGui = !mShowImGui;
	}
	if (mRunning && mShowImGui)
	{
		ImGui::SFML::ProcessEvent(event);
	}
}

void ImGuiToolManager::ImGuiMain()
{
	const size_t tabs = static_cast<size_t>(ImGuiToolTab::Count);

	if (ImGui::BeginMainMenuBar())
	{
		for (size_t i = 0; i < tabs; ++i)
		{
			if (ImGui::BeginMenu(ImGuiToolTabNames::names[i]))
			{
				const size_t size = mTools[i].size();
				for (size_t j = 0; j < size; ++j)
				{
					ImGuiTool* tool = mTools[i][j];
					ImGui::MenuItem(tool->GetName(), NULL, &tool->mVisible);
				}
				ImGui::EndMenu();
			}
		}

		if (World* world = Universe::GetInstance().GetCurrentWorld())
		{
			if (world->IsPlaying())
			{
				if (ImGui::SmallButton("" ICON_FA_PAUSE))
				{
					world->Pause();
				}
			}
			else
			{
				if (ImGui::SmallButton("" ICON_FA_PLAY))
				{
					world->Play();
				}
			}
		}

		ImGui::EndMainMenuBar();
	}

	for (size_t i = 0; i < tabs; ++i)
	{
		const size_t size = mTools[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			ImGuiTool* tool = mTools[i][j];
			if (tool->mVisible)
			{
				const bool imguiDemoTool = tool->IsImGuiDemoTool();
				if (!imguiDemoTool)
				{
					if (!ImGui::Begin(tool->GetName(), &tool->mVisible, tool->GetWindowFlags()))
					{
						ImGui::End();
						continue;
					}
				}
				tool->Display();
				if (tool->ShouldFocus())
				{
					tool->Focus();
				}
				if (tool->ShouldResize())
				{
					tool->Resize();
				}
				if (!imguiDemoTool)
				{
					ImGui::End();
				}
			}
		}
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
