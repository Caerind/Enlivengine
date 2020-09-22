#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/TypeInfo.hpp>

namespace en
{

class Time
{
public:
	static constexpr const I64 TicksPerMicrosecond = 1;
	static constexpr const I64 TicksPerMillisecond = 1000 * TicksPerMicrosecond;
	static constexpr const I64 TicksPerSecond = 1000 * TicksPerMillisecond;
	static constexpr const I64 TicksPerMinute = 60 * TicksPerSecond;
	static constexpr const I64 TicksPerHour = 60 * TicksPerMinute;
	static constexpr const I64 TicksPerDay = 24 * TicksPerHour;

public:
	constexpr Time() : mTicks(0LL) {}
	constexpr Time(I64 ticks) : mTicks(ticks) {}

	constexpr Time& Set(I32 minutes, I32 seconds) { return Set(0, 0, minutes, seconds, 0, 0); }
	constexpr Time& Set(I32 hours, I32 minutes, I32 seconds) { return Set(0, hours, minutes, seconds, 0); }
	constexpr Time& Set(I32 days, I32 hours, I32 minutes, I32 seconds) { return Set(days, hours, minutes, seconds, 0); }
	constexpr Time& Set(I32 days, I32 hours, I32 minutes, I32 seconds, I32 milliseconds, I32 microseconds = 0)
	{
		mTicks = days * TicksPerDay
			+ hours * TicksPerHour
			+ minutes * TicksPerMinute
			+ seconds * TicksPerSecond
			+ milliseconds * TicksPerMillisecond
			+ microseconds * TicksPerMicrosecond;
		return *this;
	}

	constexpr Time operator+(const Time& time) const { return Time(mTicks + time.mTicks); }
	constexpr Time& operator+=(const Time& time) { mTicks += time.mTicks; return *this; }
	constexpr Time operator-() const { return Time(-mTicks); }
	constexpr Time operator-(const Time& time) const { return Time(mTicks - time.mTicks); }
	constexpr Time& operator-=(const Time& time) { mTicks -= time.mTicks; return *this; }
	constexpr Time operator*(const Time& time) const = delete;
	constexpr Time& operator*=(const Time& time) = delete;
	constexpr Time operator/(const Time& time) const = delete;
	constexpr Time& operator/=(const Time& time) = delete;
	constexpr Time operator%(const Time& time) const { return Time(mTicks % time.mTicks); }
	constexpr Time& operator%=(const Time& time) { mTicks %= time.mTicks; return *this; }

	constexpr Time operator+(F32 scalar) const = delete;
	constexpr Time& operator+=(F32 scalar) = delete;
	constexpr Time operator-(F32 scalar) const = delete;
	constexpr Time& operator-=(F32 scalar) = delete;
	constexpr Time operator*(F32 scalar) const { return Time(static_cast<I64>(mTicks * scalar)); }
	constexpr Time& operator*=(F32 scalar) { mTicks = static_cast<I64>(mTicks * scalar); return *this; }
	constexpr Time operator/(F32 scalar) const { return Time(static_cast<I64>(mTicks / scalar)); }
	constexpr Time& operator/=(F32 scalar) { mTicks = static_cast<I64>(mTicks / scalar); return *this; }
	constexpr Time operator%(F32 scalar) const = delete;
	constexpr Time& operator%=(F32 scalar) = delete;

	constexpr bool operator==(const Time& time) const { return mTicks == time.mTicks; }
	constexpr bool operator!=(const Time& time) const { return mTicks != time.mTicks; }
	constexpr bool operator>(const Time& time) const { return mTicks > time.mTicks; }
	constexpr bool operator>=(const Time& time) const { return mTicks >= time.mTicks; }
	constexpr bool operator<(const Time& time) const { return mTicks < time.mTicks; }
	constexpr bool operator<=(const Time& time) const { return mTicks <= time.mTicks; }
	constexpr bool IsZero() const { return mTicks == 0LL; }

	constexpr Time GetDuration() const { return Time(mTicks >= 0LL ? mTicks : -mTicks); }

	constexpr F32 AsSeconds() const { return mTicks / static_cast<F32>(TicksPerSecond); }
	constexpr I32 AsMilliseconds() const { return static_cast<I32>(mTicks / TicksPerMillisecond); }
	constexpr I64 AsMicroseconds() const { return static_cast<I64>(mTicks / TicksPerMicrosecond); }
	constexpr I64 GetTicks() const { return mTicks; }

	static Time Now();
	static constexpr Time Days(I32 amount) { return Time(amount * TicksPerDay); }
	static constexpr Time Hours(I32 amount) { return Time(amount * TicksPerMinute); }
	static constexpr Time Minutes(I32 amount) { return Time(amount * TicksPerMinute); }
	static constexpr Time Seconds(F32 amount) { return Time(static_cast<I64>(amount * TicksPerSecond)); }
	static constexpr Time Milliseconds(I32 amount) { return Time(amount * TicksPerMillisecond); }
	static constexpr Time Microseconds(I64 amount) { return Time(amount * TicksPerMicrosecond); }

	static constexpr Time Zero() { return Time(0LL); }
	static constexpr Time Second() { return Time(TicksPerSecond); }
	static constexpr Time Millisecond() { return Time(TicksPerMillisecond); }
	static constexpr Time Microsecond() { return Time(TicksPerMicrosecond); }

	static void Sleep(const Time& duration);

private:
	I64 mTicks;
};

constexpr Time operator*(F32 scalar, const Time& time)
{
	return time.operator*(scalar);
}

class Clock
{
public:
	Clock();

	Time GetElapsedTime() const;
	Time Restart();

private:
	Time mStart;
};

class StopWatch
{
public:
	StopWatch();

	Time GetElapsedTime() const;
	bool IsRunning() const;

	void Start();
	void Stop();

	void Reset(); // Reset and stop
	void Restart(); // Reset and start again

private:
	Clock mClock;
	Time mTimeBuffer;
	bool mRunning;
};

class Timer
{
public:
	Timer();

	Time GetRemainingTime() const;
	bool IsRunning() const;
	bool IsExpired() const;

	void Start();
	void Stop();

	void Reset(Time time); // Reset and stop
	void Restart(Time time); // Reset and start again

private:
	StopWatch mStopWatch;
	Time mLimit;
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO(en::Time)
