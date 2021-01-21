#include <Enlivengine/Platform/Time.hpp>

#include <chrono>
#include <ctime>

namespace en
{

Time Time::Now()
{
	return Time(static_cast<I64>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()));
}

bool Time::Serialize(ClassSerializer& serializer, const char* name)
{
	return serializer.Serialize(name, mTicks);
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
	if (time >= Time::Zero())
	{
		mLimit = time;
		mStopWatch.Reset();
	}
	else
	{
		mLimit = Time::Zero();
		mStopWatch.Stop();
	}
}

void Timer::Restart(Time time)
{
	Reset(time);
	Start();
}

DateTime::DateTime()
	: mTicks(Now().mTicks)
{
}

DateTime DateTime::Now()
{
	time_t rawtime;
	time(&rawtime);
#ifdef ENLIVE_COMPILER_MSVC
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	return DateTime(
		timeinfo.tm_year + 1900,
		timeinfo.tm_mon + 1, // months since January 0-11
		timeinfo.tm_mday, // day of the month 1-31
		timeinfo.tm_hour, // hours since midnight 0-23
		timeinfo.tm_min, // minutes after the hour 0-59
		timeinfo.tm_sec // seconds after the minute 0-59
	);
#else
	struct tm* timeinfo = localtime(&rawtime);
	return DateTime(
		timeinfo->tm_year + 1900,
		timeinfo->tm_mon + 1, // months since January 0-11
		timeinfo->tm_mday, // day of the month 1-31
		timeinfo->tm_hour, // hours since midnight 0-23
		timeinfo->tm_min, // minutes after the hour 0-59
		timeinfo->tm_sec // seconds after the minute 0-59
	);
#endif // ENLIVE_COMPILER_MSVC
}

DateTime DateTime::Today()
{
	return Now().GetDate();
}

bool DateTime::Serialize(ClassSerializer& serializer, const char* name)
{
	// TODO : Improve ?
	return serializer.Serialize(name, mTicks);
}

} // namespace en