#include <Enlivengine/Tools/ImGuiToolManager.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

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

bool ImGuiTool::IsVisible() const
{
	return mVisible;
}

ImGuiToolManager::ImGuiToolManager()
	: mRunning(false)
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
	mRunning = true;
}

void ImGuiToolManager::Shutdown()
{
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
		ImGuiMain();
		ImGuiTools();
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