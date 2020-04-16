#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_ENABLE_PROFILE)

#include <Enlivengine/Application/ImGuiToolManager.hpp>
#include <Enlivengine/System/Profiler.hpp>

namespace en
{

class ImGuiProfiler : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiProfiler);

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

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
