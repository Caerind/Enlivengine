#include <Enlivengine/Tools/ImGuiToolManager.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <dear-imgui/imgui.h>

#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Graphics/ImGuiWrapper.hpp>
#include <Enlivengine/Window/Keyboard.hpp>

#ifdef ENLIVE_MODULE_CORE
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#endif // ENLIVE_MODULE_CORE

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

void ImGuiToolManager::Initialize()
{
	enAssert(ImGuiWrapper::IsInitialized());

#ifdef ENLIVE_DEBUG
	mShowImGui = true;
#else
	mShowImGui = false;
#endif // ENLIVE_DEBUG
	mRunning = true;
}

void ImGuiToolManager::Shutdown()
{
	mShowImGui = false;
	if (mRunning)
	{
		mRunning = false;
	}
}

void ImGuiToolManager::Update()
{
	ENLIVE_PROFILE_FUNCTION();

	if (mRunning)
	{
		if (Keyboard::IsPressed(Keyboard::Key::Menu))
		{
			mShowImGui = !mShowImGui;
		}

		if (mShowImGui)
		{
			ImGuiMain();
		}
	}
}

void ImGuiToolManager::ImGuiMain()
{
	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());

	if (ImGui::BeginMainMenuBar())
	{
		for (size_t i = 0; i < tabs; ++i)
		{
			std::string name(Enum::GetValueNames<ImGuiToolTab>()[i]);
			if (ImGui::BeginMenu(name.c_str()))
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

#ifdef ENLIVE_MODULE_CORE
		if (World* world = Universe::GetInstance().GetCurrentWorld())
		{
			if (world->IsPlaying())
			{
				if (ImGui::SmallButton("P" /*ICON_FA_PAUSE*/)) // TODO : FONT AWESOME
				{
					world->Pause();
				}
			}
			else
			{
				if (ImGui::SmallButton("P" /*ICON_FA_PLAY*/)) // TODO : FONT AWESOME
				{
					world->Play();
				}
			}
		}
#endif // ENLIVE_MODULE_CORE

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
#endif // ENLIVE_MODULE_TOOLS