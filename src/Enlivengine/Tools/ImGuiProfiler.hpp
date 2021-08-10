#pragma once

#include <Enlivengine/Config.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_PROFILE)

#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Tools/ImGuiTool.hpp>

namespace en
{

class ImGuiProfiler : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiProfiler);

public:
	ImGuiToolTab GetTab() const override;
	const char* GetName() const override;
	const char* GetSaveName() const override;

	void Display() override;

	// Shortcuts for Profiler
	void SetEnabled(bool enabled);
	bool IsEnabled() const;
	bool CanCurrentFrameBeCaptured() const;
	void CaptureCurrentFrame();
	void CaptureFrames(U32 nbFrames);
	bool IsCapturing() const;

	// Specific methods
	void CaptureCurrentFrameAndOpenProfiler();

private:
	void DisplayFrame(const ProfilerFrame& frame) const;

private:
	U32 mCaptureFrames;
	U32 mCurrentFrameIndex;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && ENLIVE_ENABLE_PROFILE
