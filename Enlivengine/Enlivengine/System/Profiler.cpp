#include <Enlivengine/System/Profiler.hpp>

#ifdef ENLIVE_ENABLE_PROFILE

#include <Enlivengine/System/Assert.hpp>

namespace en
{

Time ProfilerTask::GetDuration() const
{
	enAssert(start <= end);
	return end - start;
}

Time ProfilerFrame::GetDuration() const
{
	enAssert(start <= end);
	return end - start;
}

F32 ProfilerFrame::GetPercentTime(const Time& timePoint) const
{
	enAssert(start <= timePoint && timePoint <= end);
	const I64 durationMS = GetDuration().AsMicroseconds();
	const I64 delta = (timePoint - start).AsMicroseconds();
	const F64 percent = (1.0 * delta) / (1.0 * durationMS);
	return static_cast<F32>(percent);
}

F32 ProfilerFrame::GetPercentDuration(const Time& subDuration) const
{
	enAssert(subDuration <= GetDuration());
	const I64 durationMS = GetDuration().AsMicroseconds();
	const I64 subDurationMS = subDuration.AsMicroseconds();
	const F64 percent = (1.0 * subDurationMS) / (1.0 * durationMS);
	return static_cast<F32>(percent);
}

U32 ProfilerFrame::GetMaxDepth() const
{
	U32 maxDepth = 0;
	const size_t size = tasks.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (tasks[i].depth > maxDepth)
		{
			maxDepth = tasks[i].depth;
		}
	}
	return maxDepth;
}

Profile::Profile(const char* functionName)
{
	Profiler::GetInstance().StartFunction(functionName);
}

Profile::~Profile()
{
	Profiler::GetInstance().EndFunction();
}

Profiler::Profiler()
	: mEnabled(false)
	, mWasEnabledThisFrame(false)
	, mCurrentFrame()
	, mCapturing(false)
	, mCapturingFrames(0)
	, mProfilerFrames()
{
	SetFrameCapacity(kDefaultFramesCapacity);
	mCurrentFrame.tasks.reserve(kProfilesPerFrameCapacity);
	mIndexStack.reserve(kMaxDepthCapacity);
}

void Profiler::SetFrameCapacity(U32 capacity)
{
	mProfilerFrames.reserve(capacity);
}

U32 Profiler::GetFrameCapacity() const
{
	return static_cast<U32>(mProfilerFrames.capacity());
}

void Profiler::SetEnabled(bool enabled)
{
	if (mEnabled != enabled)
	{
		mEnabled = enabled;
		if (mEnabled)
		{
			mWasEnabledThisFrame = true;
		}
		else
		{
			mCurrentFrame.tasks.clear();
			mCurrentFrame.tasks.reserve(kProfilesPerFrameCapacity);
			mIndexStack.reserve(kMaxDepthCapacity);
			mIndexStack.clear();
		}
	}
}

bool Profiler::IsEnabled() const
{
	return mEnabled;
}

bool Profiler::CanCurrentFrameBeCaptured() const
{
	return IsEnabled() && !mWasEnabledThisFrame;
}

void Profiler::CaptureCurrentFrame()
{
	enAssert(CanCurrentFrameBeCaptured());
	CaptureFrames(1);
}

void Profiler::CaptureFrames(U32 nbFrames)
{
	if (CanCurrentFrameBeCaptured() && nbFrames > 0)
	{
		mCapturing = true;
		mCapturingFrames = nbFrames;
		mProfilerFrames.clear();
		mProfilerFrames.resize(nbFrames);
	}
}

bool Profiler::IsCapturing() const
{
	return mCapturing;
}

const std::vector<ProfilerFrame>& Profiler::GetProfilerFrames() const
{
	enAssert(!IsCapturing());
	return mProfilerFrames;
}

void Profiler::StartFrame(U32 frameNumber)
{
	const Time frameStart = Time::Now();

	if (IsEnabled() && mWasEnabledThisFrame)
	{
		mWasEnabledThisFrame = false;
	}

	ENLIVE_PROFILE_FUNCTION();

	if (CanCurrentFrameBeCaptured())
	{
		// 1 because of the ENLIVE_PROFILE_FUNCTION already opened
		enAssert(mCurrentFrame.tasks.size() == 1);
		enAssert(mIndexStack.size() == 1);

		mCurrentFrame.frame = frameNumber;
		mCurrentFrame.start = frameStart;
		mCurrentFrame.end = frameStart;
	}
}

void Profiler::EndFrame()
{
	StartFunction("en::Profiler::EndFrame");

	if (CanCurrentFrameBeCaptured())
	{
		mCurrentFrame.end = Time::Now();
		EndFunction();

		enAssert(mIndexStack.size() == 0);

		if (IsCapturing())
		{
			const U32 index = static_cast<U32>(mProfilerFrames.size()) - mCapturingFrames;
			mProfilerFrames[index] = std::move(mCurrentFrame);
			mCapturingFrames--;
			if (mCapturingFrames == 0)
			{
				mCapturing = false;
			}
		}

		mCurrentFrame.tasks.clear();
		mCurrentFrame.tasks.reserve(kProfilesPerFrameCapacity);
		mIndexStack.clear();
		mIndexStack.reserve(kMaxDepthCapacity);
	}
	else
	{
		EndFunction();
	}
}

void Profiler::StartFunction(const char* name)
{
	if (CanCurrentFrameBeCaptured())
	{
		ProfilerTask task;
		task.name = name;
		task.start = Time::Now();
		task.depth = GetCurrentDepth();
		mCurrentFrame.tasks.push_back(task);
		mIndexStack.push_back(static_cast<U32>(mCurrentFrame.tasks.size() - 1));
	}
}

void Profiler::EndFunction()
{
	if (CanCurrentFrameBeCaptured())
	{
		enAssert(mIndexStack.size() > 0);
		const U32 index = mIndexStack.back();
		mCurrentFrame.tasks[index].end = Time::Now();
		mIndexStack.pop_back();
	}
}

U32 Profiler::GetCurrentDepth() const
{
	return static_cast<U32>(mIndexStack.size());
}

/*
void ConsoleProfiler::DisplayFrame(const ProfilerFrame& frame)
{
	ENLIVE_PROFILE_FUNCTION();

	// TODO : Move this elsewhere ?
    #ifdef ENLIVE_COMPILER_MSVC
        system("cls");
	#else
        printf("\e[1;1H\e[2J"); // Should work on ANSI
	#endif

	printf("Frame %u, duration : %I64dus\n", frame.frame, frame.GetDuration().asMicroseconds());
	for (const auto& task : frame.tasks)
	{
		const U32 depth = task.depth;
		for (U32 i = 0; i < depth; ++i)
		{
			printf("  ");
		}
		printf("-%s, duration : %I64dus, percent : %f\n", task.name, task.GetDuration().asMicroseconds(), frame.GetPercent(task.GetDuration()));
	}
}
*/

} // namespace en

#endif // ENLIVE_ENABLE_PROFILE
