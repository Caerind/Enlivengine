#include <Enlivengine/Tools/ImGuiToolManager.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

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
#include <Enlivengine/Tools/ImGuiInputEditor.hpp>
#include <Enlivengine/Tools/ImGuiLogger.hpp>
#include <Enlivengine/Tools/ImGuiMemoryDebugger.hpp>
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

#ifdef ENLIVE_TOOL
bool ImGuiToolManager::CreateDefaultToolsFile(const std::string& filename)
{
	std::ofstream file(filename);
	if (!file)
	{
		return false;
	}

	file << "{" << std::endl;
	file << "    \"" << ImGuiEditor::GetInstance().GetSaveName()			<< "\": true" << "," << std::endl;
	file << "    \"" << ImGuiEntityBrowser::GetInstance().GetSaveName()		<< "\": true" << "," << std::endl;
	file << "    \"" << ImGuiLogger::GetInstance().GetSaveName()			<< "\": true" << "," << std::endl;
	file << "    \"" << ImGuiResourceBrowser::GetInstance().GetSaveName()	<< "\": true" << "," << std::endl;
	file << "    \"" << ImGuiWorlds::GetInstance().GetSaveName()			<< "\": true" << std::endl;
	file << "}" << std::endl;

	file.close();

	return true;
}

bool ImGuiToolManager::CreateDefaultImGuiFile(const std::string& filename)
{
	std::ofstream file(filename);
	if (!file)
	{
		return false;
	}

	{
		file << "[Window][DockSpaceViewport_11111111]" << std::endl;
		file << "Pos=0,24" << std::endl;
		file << "Size=1920,993" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << std::endl;
		file << "[Window][Debug##Default]" << std::endl;
		file << "Pos=60,60" << std::endl;
		file << "Size=400,400" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << std::endl;
	}
	{
		file << "[Window][" << ImGuiLogger::GetInstance().GetName() << "]" << std::endl;
		file << "Pos=441,806" << std::endl;
		file << "Size=1479,211" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << "DockId=0x00000004,0" << std::endl;
		file << std::endl;
	}
	{
		file << "[Window][" << ImGuiResourceBrowser::GetInstance().GetName() << "]" << std::endl;
		file << "Pos=0,24" << std::endl;
		file << "Size=439,993" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << "DockId=0x00000001,2" << std::endl;
		file << std::endl;
	}
	{
		file << "[Window][" << ImGuiEntityBrowser::GetInstance().GetName() << "]" << std::endl;
		file << "Pos=0,24" << std::endl;
		file << "Size=439,993" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << "DockId=0x00000001,1" << std::endl;
		file << std::endl;
	}
	{
		file << "[Window][" << ImGuiWorlds::GetInstance().GetName() << "]" << std::endl;
		file << "Pos=0,24" << std::endl;
		file << "Size=439,993" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << "DockId=0x00000001,0" << std::endl;
		file << std::endl;
	}
	{
		file << "[Window][" << ImGuiEditor::GetInstance().GetName() << "]" << std::endl;
		file << "Pos=441,24" << std::endl;
		file << "Size=1479,780" << std::endl;
		file << "Collapsed=0" << std::endl;
		file << "DockId=0x00000003,0" << std::endl;
		file << std::endl;
	}
	{
		file << "[Docking][Data]" << std::endl;
		file << "DockSpace     ID=0x8B93E3BD Window=0xA787BDB4 Pos=0,24 Size=1920,993 Split=X" << std::endl;
		file << "  DockNode    ID=0x00000001 Parent=0x8B93E3BD SizeRef=439,993 Selected=0x426E72EE" << std::endl;
		file << "  DockNode    ID=0x00000002 Parent=0x8B93E3BD SizeRef=1479,993 Split=Y Selected=0xCB5DF48C" << std::endl;
		file << "    DockNode  ID=0x00000003 Parent=0x00000002 SizeRef=1620,780 Selected=0xCB5DF48C" << std::endl;
		file << "    DockNode  ID=0x00000004 Parent=0x00000002 SizeRef=1620,211 Selected=0x893FB60F" << std::endl;
		file << std::endl;
	}

	file << std::endl;
	file.close();

	return true;
}
#endif // ENLIVE_TOOL

void ImGuiToolManager::Update(Window& window)
{
	ENLIVE_PROFILE_FUNCTION();

	ImGuiToolManager& imgui = GetInstance();
	enAssert(imgui.mInitialized);

#if defined(ENLIVE_RELEASE) && defined(ENLIVE_DEBUG)
	if (Keyboard::IsPressed(Keyboard::Key::Application))
	{
		imgui.mRunning = !imgui.mRunning;
	}
#endif // ENLIVE_RELEASE && ENLIVE_DEBUG

	if (imgui.mRunning)
	{
		const Vector2u windowSize = window.GetSize();
		ImGuiWrapper::BeginFrame(250, windowSize.x, windowSize.y, Time::GetDeltaTime().AsSeconds());
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
#endif // ENLIVE_TOOL

	RegisterTool(ImGuiLogger::GetInstance());
	RegisterTool(ImGuiMemoryDebugger::GetInstance());
	RegisterTool(ImGuiProfiler::GetInstance());
	RegisterTool(ImGuiConsole::GetInstance());
	RegisterTool(ImGuiDemoWindow::GetInstance());
	
	RegisterTool(ImGuiInputEditor::GetInstance());
	RegisterTool(ImGuiAnimationEditor::GetInstance());
	RegisterTool(ImGuiResourceBrowser::GetInstance());

	RegisterTool(ImGuiWorlds::GetInstance());
	RegisterTool(ImGuiEntityBrowser::GetInstance());
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