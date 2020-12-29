#include <Enlivengine/Tools/ImGuiToolManager.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <iomanip>

#include <json/json.hpp>

#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

#if defined(ENLIVE_RELEASE) && defined(ENLIVE_DEBUG)
#include <Enlivengine/Window/Keyboard.hpp>
#endif // ENLIVE_RELEASE && ENLIVE_DEBUG

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
#include <Enlivengine/Tools/ImGuiWorlds.hpp>

namespace en
{

bool ImGuiToolManager::Initialize()
{
	ImGuiToolManager& imgui = GetInstance();
	enAssert(!imgui.mInitialized);

	imgui.mInitialized = true;

#ifdef ENLIVE_TOOL
	imgui.mRunning = true;
#endif // ENLIVE_TOOL

	imgui.RegisterTools();

	return true;
}

bool ImGuiToolManager::IsInitialized()
{
	return GetInstance().mInitialized;
}

bool ImGuiToolManager::Release()
{
	ImGuiToolManager& imgui = GetInstance();
	enAssert(imgui.mInitialized);

	imgui.mRunning = false;

	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
	for (size_t i = 0; i < tabs; ++i)
	{
		const size_t size = imgui.mTools[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			imgui.mTools[i][j]->Release();
		}
	}

	imgui.mInitialized = false;

	return true;
}

bool ImGuiToolManager::LoadFromFile(const std::string& filename)
{
	nlohmann::json jsonInput;

	{
		std::ifstream file(filename);
		if (!file)
		{
			return false;
		}
		// Don't know why, but it seems the operator>> is not recognized...
		nlohmann::detail::parser<nlohmann::json>(nlohmann::detail::input_adapter(file)).parse(false, jsonInput);
		file.close();
	}

	ImGuiToolManager& imgui = GetInstance();
	enAssert(imgui.mInitialized);

	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
	for (size_t i = 0; i < tabs; ++i)
	{
		const size_t size = imgui.mTools[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			if (ImGuiTool* tool = imgui.mTools[i][j])
			{
				auto itr = jsonInput.find(tool->GetSaveName());
				if (itr != jsonInput.end() && itr->is_boolean())
				{
					tool->mVisible = itr.value();
				}
			}
		}
	}

	return true;
}

bool ImGuiToolManager::SaveToFile(const std::string& filename)
{
	nlohmann::json jsonInput;

	ImGuiToolManager& imgui = GetInstance();
	enAssert(imgui.mInitialized);

	constexpr size_t tabs = static_cast<size_t>(Enum::GetCount<ImGuiToolTab>());
	for (size_t i = 0; i < tabs; ++i)
	{
		const size_t size = imgui.mTools[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			if (ImGuiTool* tool = imgui.mTools[i][j])
			{
				jsonInput[tool->GetSaveName()] = tool->mVisible;
			}
		}
	}

	{
		std::ofstream file(filename);
		if (!file)
		{
			return false;
		}

		file << std::setw(4) << jsonInput;

		file.close();
	}

	return true;
}

void ImGuiToolManager::Update(Window& window, const Time& dt)
{
	ENLIVE_PROFILE_FUNCTION();

	ImGuiToolManager& imgui = GetInstance();
	enAssert(imgui.mInitialized);

#if defined(ENLIVE_RELEASE) && defined(ENLIVE_DEBUG)
	if (Keyboard::IsPressed(Keyboard::Key::F1) && Keyboard::IsControlHold())
	{
		imgui.mRunning = !imgui.mRunning;
	}
#endif // ENLIVE_RELEASE && ENLIVE_DEBUG

	if (imgui.mRunning)
	{
		const Vector2u windowSize = window.GetSize();
		ImGuiWrapper::BeginFrame(250, windowSize.x, windowSize.y, dt.AsSeconds());
		imgui.ImGuiMain();
		imgui.ImGuiTools();
		ImGuiWrapper::EndFrame();
	}
}

void ImGuiToolManager::RegisterTool(ImGuiTool& tool)
{
	ImGuiToolManager& imgui = GetInstance();
	enAssert(imgui.mInitialized);

	const U32 tab = static_cast<U32>(tool.GetTab());

	std::vector<ImGuiTool*>& tools = imgui.mTools[tab];

#ifdef ENLIVE_ENABLE_ASSERT
	const U32 toolHash = Hash::SlowHash(tool.GetName());
	const size_t size = tools.size();
	for (size_t i = 0; i < size; ++i)
	{
		enAssert(Hash::SlowHash(tools[i]->GetName()) != toolHash);
	}
#endif // ENLIVE_ENABLE_ASSERT

	tool.Initialize();

	tools.push_back(&tool);
}

void ImGuiToolManager::RegisterTools()
{
#ifdef ENLIVE_TOOL
	RegisterTool(ImGuiEditor::GetInstance());
	RegisterTool(ImGuiGame::GetInstance());
#endif // ENLIVE_TOOL

	RegisterTool(ImGuiWorlds::GetInstance());
	RegisterTool(ImGuiAnimationEditor::GetInstance());
	RegisterTool(ImGuiConsole::GetInstance());
	RegisterTool(ImGuiDemoWindow::GetInstance());
	RegisterTool(ImGuiEntityBrowser::GetInstance());
	RegisterTool(ImGuiInputEditor::GetInstance());
	RegisterTool(ImGuiLogger::GetInstance());
	RegisterTool(ImGuiMemoryDebugger::GetInstance());
	RegisterTool(ImGuiPhysic::GetInstance());
	RegisterTool(ImGuiProfiler::GetInstance());
	RegisterTool(ImGuiResourceBrowser::GetInstance());
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

#ifdef ENLIVE_TOOL
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
#endif // ENLIVE_TOOL
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

ImGuiToolManager& ImGuiToolManager::GetInstance()
{
	static ImGuiToolManager instance;
	return instance;
}

ImGuiToolManager::ImGuiToolManager()
	: mTools()
	, mRunning(false)
{
}

ImGuiToolManager::~ImGuiToolManager()
{
	enAssert(!mRunning);
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI