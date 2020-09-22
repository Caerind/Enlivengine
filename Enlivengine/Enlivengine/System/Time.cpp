#include <Enlivengine/System/Time.hpp>

#include <chrono>
#include <thread>

#include <Enlivengine/System/Assert.hpp>

namespace en
{

Time Time::Now()
{
	return Time(static_cast<I64>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()));
}

void Time::Sleep(const Time& duration)
{
	if (duration <= Zero())
		return;

	std::this_thread::sleep_for(std::chrono::microseconds(duration.AsMicroseconds()));
}

Clock::Clock()
	: mStart(Time::Now())
{
}

Time Clock::GetElapsedTime() const
{
	return Time::Now() - mStart;
}

Time Clock::Restart()
{
	const Time now = Time::Now();
	const Time elapsed = now - mStart;
	mStart = now;
	return elapsed;
}

StopWatch::StopWatch()
	: mClock()
	, mTimeBuffer(Time::Zero())
	, mRunning(false)
{
}

Time StopWatch::GetElapsedTime() const
{
	if (mRunning)
		return mTimeBuffer + mClock.GetElapsedTime();
	else
		return mTimeBuffer;
}

bool StopWatch::IsRunning() const
{
	return mRunning;
}

void StopWatch::Start()
{
	if (!mRunning)
	{
		mRunning = true;
		mClock.Restart();
	}
}

void StopWatch::Stop()
{
	if (mRunning)
	{
		mRunning = false;
		mTimeBuffer += mClock.GetElapsedTime();
	}
}

void StopWatch::Reset()
{
	mTimeBuffer = Time::Zero();
	mRunning = false;
}

void StopWatch::Restart()
{
	Reset();
	Start();
}

Timer::Timer()
	: mStopWatch()
	, mLimit(Time::Zero())
{
}

Time Timer::GetRemainingTime() const
{
	const Time delta = mLimit - mStopWatch.GetElapsedTime();
	return (delta > Time::Zero()) ? delta : Time::Zero();
}

bool Timer::IsRunning() const
{
	return mStopWatch.IsRunning() && !IsExpired();
}

bool Timer::IsExpired() const
{
	return mStopWatch.GetElapsedTime() >= mLimit;
}

void Timer::Start()
{
	mStopWatch.Start();
}

void Timer::Stop()
{
	mStopWatch.Stop();
}

void Timer::Reset(Time time)
{
	enAssert(time >= Time::Zero());
	mLimit = time;
	mStopWatch.Reset();
}

void Timer::Restart(Time time)
{
	Reset(time);
	Start();
}

} // namespace en
