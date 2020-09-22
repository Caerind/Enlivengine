#include <Enlivengine/Tools/ImGuiConsole.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>
#include <Enlivengine/System/String.hpp>

namespace en
{

ImGuiConsole::ImGuiConsole()
	: ImGuiTool()
	, mInputBuffer()
	, mLines()
	, mMaxSize(kDefaultMaxSize)
{
}

ImGuiConsole::~ImGuiConsole()
{
}

ImGuiToolTab ImGuiConsole::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiConsole::GetName() const
{
	return ICON_FA_TERMINAL " Console";
}

void ImGuiConsole::Display()
{
	if (ImGui::Button("Clear"))
	{
		mLines.clear();
		mLines.reserve(mMaxSize);
	}

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();

	// Command-line
	if (ImGui::InputText("Input", mInputBuffer, kMaxInputBufSize, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		AddLine(std::string(mInputBuffer));

#ifdef ENLIVE_COMPILER_MSVC
		strcpy_s(mInputBuffer, "");
#else
		strcpy(mInputBuffer, "");
#endif // ENLIVE_COMPILER_MSVC
	}
	// Keep auto focus on the input box
	if (ImGui::IsItemHovered() || (ImGui::IsAnyWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
	{
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	}

	ImGui::BeginChild("", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	for (U32 i = 0; i < mLines.size(); ++i)
	{
		ImVec4 col;
		if (mLines[i].find("[error]") != std::string::npos)
		{
			col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		}
		else if (mLines[i].find("# ") != std::string::npos)
		{
			ImGui::Spacing();
			col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		}
		else
		{
			col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(mLines[i].c_str());
		ImGui::PopStyleColor();
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void ImGuiConsole::AddLine(const std::string& line)
{
	if (GetCurrentSize() + 1 > GetMaxSize())
	{
		mLines.erase(mLines.begin());
	}

	mLines.push_back(line);

	AskForResize();
}

void ImGuiConsole::SetMaxSize(U32 size)
{
	mMaxSize = size;
}

U32 ImGuiConsole::GetMaxSize() const
{
	return mMaxSize;
}

U32 ImGuiConsole::GetCurrentSize() const
{
	return static_cast<U32>(mLines.size());
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI