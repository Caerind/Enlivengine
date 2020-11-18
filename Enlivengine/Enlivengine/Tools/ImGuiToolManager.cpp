#include <Enlivengine/Tools/ImGuiToolManager.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

// Tools
#include <Enlivengine/Tools/ImGuiAnimationEditor.hpp>
#include <Enlivengine/Tools/ImGuiConsole.hpp>
#include <Enlivengine/Tools/ImGuiDemoWindow.hpp>
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>
#include <Enlivengine/Tools/ImGuiGame.hpp>
#include <Enlivengine/Tools/ImGuiInputEditor.hpp>
#include <Enlivengine/Tools/ImGuiLogger.hpp>
#include <Enlivengine/Tools/ImGuiMemoryDebugger.hpp>
#include <Enlivengine/Tools/ImGuiPhysic.hpp>
#include <Enlivengine/Tools/ImGuiProfiler.hpp>
#include <Enlivengine/Tools/ImGuiResourceBrowser.hpp>

namespace en
{

ImGuiToolManager::ImGuiToolManager()
	: mRunning(false)
{
}

void ImGuiToolManager::Initialize()
{
	enAssert(ImGuiWrapper::IsInitialized());
	enAssert(!mRunning);

	RegisterTools();

	mRunning = true;

	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
	for (size_t i = 0; i < tabs; ++i)
	{
		const size_t size = mTools[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			mTools[i][j]->Initialize();
		}
	}
}

void ImGuiToolManager::Release()
{
	enAssert(mRunning);

	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
	for (size_t i = 0; i < tabs; ++i)
	{
		const size_t size = mTools[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			mTools[i][j]->Release();
		}
	}

	mRunning = false;
}

void ImGuiToolManager::Update()
{
	ENLIVE_PROFILE_FUNCTION();

	if (mRunning)
	{
		ImGuiMain();
		ImGuiTools();
	}
}

void ImGuiToolManager::RegisterTools()
{
	enAssert(!mRunning);

	RegisterTool(ImGuiAnimationEditor::GetInstance());
	RegisterTool(ImGuiConsole::GetInstance());
	RegisterTool(ImGuiDemoWindow::GetInstance());
	RegisterTool(ImGuiEditor::GetInstance());
	RegisterTool(ImGuiEntityBrowser::GetInstance());
	RegisterTool(ImGuiGame::GetInstance());
	RegisterTool(ImGuiInputEditor::GetInstance());
	RegisterTool(ImGuiLogger::GetInstance());
	RegisterTool(ImGuiMemoryDebugger::GetInstance());
	RegisterTool(ImGuiPhysic::GetInstance());
	RegisterTool(ImGuiProfiler::GetInstance());
	RegisterTool(ImGuiResourceBrowser::GetInstance());
}

void ImGuiToolManager::RegisterTool(ImGuiTool& tool)
{
	const U32 tab = static_cast<U32>(tool.GetTab());

	std::vector<ImGuiTool*>& tools = mTools[tab];

#ifdef ENLIVE_ENABLE_ASSERT
	const U32 toolHash = Hash::SlowHash(tool.GetName());
	const size_t size = tools.size();
	for (size_t i = 0; i < size; ++i)
	{
		enAssert(Hash::SlowHash(tools[i]->GetName()) != toolHash);
	}
#endif // ENLIVE_ENABLE_ASSERT

	tools.push_back(&tool);
}

void ImGuiToolManager::ImGuiMain()
{
	if (ImGui::BeginMainMenuBar())
	{
		constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
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

		ImGui::EndMainMenuBar();
	}

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void ImGuiToolManager::ImGuiTools()
{
	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
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