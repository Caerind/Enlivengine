#include <Enlivengine/Tools/ImGuiLogger.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_LOG)

#include <imgui/imgui.h>
#include <Enlivengine/System/String.hpp>

namespace en
{

ImGuiLogger::ImGuiLogger()
	: ImGuiTool()
	, Logger()
	, mMessages()
	, mMaxSize(kDefaultMaxSize)
{
}

ImGuiLogger::~ImGuiLogger()
{
	UnregisterLogger();
}

ImGuiToolTab ImGuiLogger::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiLogger::GetName() const
{
	return ICON_FA_LIST_UL " Logger";
}

void ImGuiLogger::Display()
{
	if (ImGui::Button("Clear"))
	{
		mMessages.clear();
		mMessages.reserve(mMaxSize);
	}

	ImGui::BeginChild("", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	for (U32 i = 0; i < mMessages.size(); ++i)
	{
		ImVec4 col;
		if (mMessages[i].type == LogType::Error || mMessages[i].type == LogType::Fatal)
		{
			col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		}
		else if (mMessages[i].type == LogType::Warning)
		{
			col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		}
		else
		{
			col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(mMessages[i].message.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void ImGuiLogger::Write(const LogMessage& message)
{
	if (GetCurrentSize() + 1 > GetMaxSize())
	{
		mMessages.erase(mMessages.begin());
	}
	mMessages.push_back(message);
	AskForResize();
}

void ImGuiLogger::SetMaxSize(U32 size)
{
	mMaxSize = size;
}

U32 ImGuiLogger::GetMaxSize() const
{
	return mMaxSize;
}

U32 ImGuiLogger::GetCurrentSize() const
{
	return static_cast<U32>(mMessages.size());
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_LOG