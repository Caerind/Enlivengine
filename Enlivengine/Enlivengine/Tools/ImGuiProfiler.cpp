#include <Enlivengine/Tools/ImGuiProfiler.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_PROFILE)

#include <imgui/imgui.h>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/Math/Color.hpp>

namespace en
{

ImGuiProfiler::ImGuiProfiler()
	: ImGuiTool()
	, mCaptureFrames(1)
	, mCurrentFrameIndex(0)
{
}

ImGuiToolTab ImGuiProfiler::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiProfiler::GetName() const
{
	return ICON_FA_CLOCK " Profiler";
}

void ImGuiProfiler::Display()
{
	if (ImGui::Button(IsEnabled() ? "Disable Profiler" : "Enable Profiler"))
	{
		SetEnabled(!IsEnabled());
		AskForResize();
	}

	if (IsEnabled())
	{
		ImGui::Separator();

		if (ImGui::Button("Capture"))
		{
			Profiler::GetInstance().CaptureFrames(mCaptureFrames);
			AskForResize();
		}

		ImGui::SameLine();

		enAssert(mCaptureFrames >= 1);
		int captureFrames = static_cast<int>(mCaptureFrames);
		if (ImGui::InputInt("NbFrames", &captureFrames))
		{
			if (captureFrames < 1)
			{
				captureFrames = 1;
			}
			mCaptureFrames = static_cast<U32>(captureFrames);
		}
	}



	ImGui::Separator();

	if (!IsCapturing())
	{
		const std::vector<ProfilerFrame>& frames = Profiler::GetInstance().GetProfilerFrames();

		if (frames.size() > 1)
		{
			ImGui::Text("Frame : %d / %d", mCurrentFrameIndex + 1, (int)frames.size());
			ImGui::SameLine();
			if (ImGui::Button("-"))
			{
				if (mCurrentFrameIndex >= 1)
				{
					mCurrentFrameIndex--;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				if (mCurrentFrameIndex < static_cast<U32>(frames.size() - 1))
				{
					mCurrentFrameIndex++;
				}
			}
		}
		else
		{
			mCurrentFrameIndex = 0;
		}

		if (frames.size() > 0)
		{
			DisplayFrame(frames[mCurrentFrameIndex]);
		}
	}
	else
	{
		ImGui::Text("Capturing...");
		AskForResize();
	}

	ImGui::Separator();
}

void ImGuiProfiler::CaptureCurrentFrameAndOpenProfiler()
{
	if (Profiler::GetInstance().CanCurrentFrameBeCaptured())
	{
		Profiler::GetInstance().CaptureCurrentFrame();
		mVisible = true;
		AskForResize();
	}
}

void ImGuiProfiler::SetEnabled(bool enabled)
{
	Profiler::GetInstance().SetEnabled(enabled);
}

bool ImGuiProfiler::IsEnabled() const
{
	return Profiler::GetInstance().IsEnabled();
}

bool ImGuiProfiler::CanCurrentFrameBeCaptured() const
{
	return Profiler::GetInstance().CanCurrentFrameBeCaptured();
}

void ImGuiProfiler::CaptureCurrentFrame()
{
	Profiler::GetInstance().CaptureCurrentFrame();
}

void ImGuiProfiler::CaptureFrames(U32 nbFrames)
{
	Profiler::GetInstance().CaptureFrames(nbFrames);
}

bool ImGuiProfiler::IsCapturing() const
{
	return Profiler::GetInstance().IsCapturing();
}

void ImGuiProfiler::DisplayFrame(const ProfilerFrame& frame) const
{
	const Time duration = frame.GetDuration();
	const I64 us = duration.AsMicroseconds();
	const F32 ms = static_cast<F32>(us) * 0.001f;

	ImGui::Spacing();

	ImGui::Text("Frame %d, duration : %5.3f ms", frame.frame, ms);

	const U32 maxDepth = frame.GetMaxDepth();
	const F32 taskHeight = 30.0f;

	const F32 currentWindowWidth = ImGui::GetWindowWidth();
	const F32 minWindowWidth = 700.0f;
	const F32 frameSize = ((currentWindowWidth > minWindowWidth) ? currentWindowWidth : minWindowWidth) - 32.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	const U32 tasks = static_cast<U32>(frame.tasks.size());
	for (U32 level = 0; level <= maxDepth; ++level)
	{
		Time levelTime = frame.start;
		if (level > 0)
		{
			ImGui::NewLine();
		}

		for (U32 i = 0; i < tasks; ++i)
		{
			const ProfilerTask& task = frame.tasks[i];
			if (task.depth == level)
			{
				const Time taskDuration = task.GetDuration();
				const F32 taskDurationMs = static_cast<F32>(taskDuration.AsMicroseconds()) * 0.001f;

				const Time prevTime = frame.start + task.start - levelTime;
				const F32 invisibleWidth = frame.GetPercentTime(prevTime) * frameSize;
				if (invisibleWidth >= 1.0f)
				{
					ImGui::InvisibleButton("", ImVec2(invisibleWidth, taskHeight));
					ImGui::SameLine();
				}

				const F32 percent = frame.GetPercentDuration(taskDuration);
				const F32 width = percent * frameSize;
				if (width >= 1.0f)
				{
					const U32 taskNameHash = Hash::SlowHash(task.name);
					const Color color(taskNameHash);
					const ImVec4 imColor = color.toImGuiColor();

					ImGui::PushStyleColor(ImGuiCol_Button, imColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, imColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, imColor);
					ImGui::Button(task.name, ImVec2(width, taskHeight));
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("%s\nDuration: %5.3f ms\n%5.3f%% of frame", task.name, taskDurationMs, percent * 100.0f);
					}
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
				}

				levelTime = task.end;
			}
		}
	}
	ImGui::PopStyleVar();
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_PROFILE
