#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_PROFILE

#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/Singleton.hpp>
#include <Enlivengine/System/Time.hpp>

#include <vector>

namespace en
{

struct ProfilerTask
{
	const char* name;
	Time start;
	Time end;
	U32 depth;

	Time GetDuration() const;
};

struct ProfilerFrame
{
	U32 frame;
	Time start;
	Time end;
	std::vector<ProfilerTask> tasks;

	Time GetDuration() const;
	F32 GetPercentTime(const Time& timePoint) const;
	F32 GetPercentDuration(const Time& subDuration) const;
	U32 GetMaxDepth() const;
};

class Profile
{
public:
	Profile(const char* functionName);
	~Profile();
};

class Profiler
{
	ENLIVE_SINGLETON(Profiler);

public:
	void SetFrameCapacity(U32 capacity);
	U32 GetFrameCapacity() const;

	void SetEnabled(bool enabled);
	bool IsEnabled() const;

	bool CanCurrentFrameBeCaptured() const;

	void CaptureCurrentFrame();
	void CaptureFrames(U32 nbFrames);
	bool IsCapturing() const;

	const std::vector<ProfilerFrame>& GetProfilerFrames() const;

private:
	friend class Application;
	void StartFrame(U32 frameNumber);
	void EndFrame();

	friend class Profile;
	void StartFunction(const char* name);
	void EndFunction();

	U32 GetCurrentDepth() const;

	static constexpr U32 kDefaultFramesCapacity{ 10 };
	static constexpr U32 kProfilesPerFrameCapacity{ 256 };
	static constexpr U32 kMaxDepthCapacity{ 10 };

private:
	bool mEnabled;
	bool mWasEnabledThisFrame;
	ProfilerFrame mCurrentFrame;
	std::vector<U32> mIndexStack;

	bool mCapturing;
	U32 mCapturingFrames;
	std::vector<ProfilerFrame> mProfilerFrames;
};

#define ENLIVE_PROFILE_FUNCTION() en::Profile functionProfile(ENLIVE_FUNCTION);
#define ENLIVE_PROFILE_SCOPE(name) en::Profile scope##name(#name);

} // namespace en

#else

#define ENLIVE_PROFILE_FUNCTION()
#define ENLIVE_PROFILE_SCOPE(name)

#endif // ENLIVE_ENABLE_PROFILE