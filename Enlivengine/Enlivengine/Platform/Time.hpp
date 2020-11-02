#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_PLATFORM

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

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
	constexpr I64 AsMicroseconds() const { return mTicks / TicksPerMicrosecond; }
	constexpr I64 GetTicks() const { return mTicks; }

	static Time Now();
	static constexpr Time Days(I32 amount) { return Time(amount * TicksPerDay); }
	static constexpr Time Hours(I32 amount) { return Time(amount * TicksPerHour); }
	static constexpr Time Minutes(I32 amount) { return Time(amount * TicksPerMinute); }
	static constexpr Time Seconds(F32 amount) { return Time(static_cast<I64>(amount) * TicksPerSecond); }
	static constexpr Time Milliseconds(I32 amount) { return Time(amount * TicksPerMillisecond); }
	static constexpr Time Microseconds(I64 amount) { return Time(amount * TicksPerMicrosecond); }

	static constexpr Time Zero() { return Time(0LL); }
	static constexpr Time Second() { return Time(TicksPerSecond); }
	static constexpr Time Millisecond() { return Time(TicksPerMillisecond); }
	static constexpr Time Microsecond() { return Time(TicksPerMicrosecond); }

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

class DateTime
{
public:
	enum class Month
	{
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	enum class Day
	{
		Monday = 1,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
		Sunday
	};

public:
	DateTime();
	constexpr DateTime(I64 ticks) : mTicks(ticks) {}
	constexpr DateTime(I32 year, I32 month, I32 day, I32 hour = 0, I32 minute = 0, I32 second = 0) : mTicks(0LL) { Set(year, month, day, hour, minute, second); }

	constexpr DateTime& Set(I64 ticks) { mTicks = ticks; return *this; }
	constexpr DateTime& Set(I32 year, I32 month, I32 day, I32 hour = 0, I32 minute = 0, I32 second = 0)
	{
		if (!Validate(year, month, day, hour, minute, second))
		{
			mTicks = 0LL;
		}
		else
		{
			constexpr I32 daysToMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
			I32 days = 0;
			if ((month > 2) && IsLeapYear(year))
			{
				days++;
			}
			year--;
			month--;
			days += year * 365;
			days += year / 4;
			days -= year / 100;
			days += year / 400;
			days += daysToMonth[month];
			days += day - 1;
			mTicks = days * Time::TicksPerDay + hour * Time::TicksPerHour + minute * Time::TicksPerMinute + second * Time::TicksPerSecond;
		}
		return *this;
	}
	constexpr DateTime& FromJulianDay(F64 julianDay) { mTicks = static_cast<I64>((julianDay - 1721425.5) * Time::TicksPerDay); return *this; }
	constexpr DateTime& FromUnixTimestamp(I64 unixTime) { mTicks = UnixDate().mTicks + unixTime * Time::TicksPerSecond; return *this; }

	constexpr DateTime operator+(const Time& time) const { return DateTime(mTicks + time.GetTicks()); }
	constexpr DateTime& operator+=(const Time& time) { mTicks += time.GetTicks(); return *this; }
	constexpr Time operator-(const DateTime& date) const { return Time(mTicks - date.mTicks); }
	constexpr DateTime operator-(const Time& time) const { return DateTime(mTicks - time.GetTicks()); }
	constexpr DateTime& operator-=(const Time& time) { mTicks -= time.GetTicks(); return *this; }

	constexpr bool operator==(const DateTime& date) const { return mTicks == date.mTicks; }
	constexpr bool operator!=(const DateTime& date) const { return mTicks != date.mTicks; }
	constexpr bool operator>(const DateTime& date) const { return mTicks > date.mTicks; }
	constexpr bool operator>=(const DateTime& date) const { return mTicks >= date.mTicks; }
	constexpr bool operator<(const DateTime& date) const { return mTicks < date.mTicks; }
	constexpr bool operator<=(const DateTime& date) const { return mTicks <= date.mTicks; }
	bool IsPresent() const { return mTicks == Now().mTicks; }
	bool IsPast() const { return mTicks < Now().mTicks; }
	bool IsFuture() const { return mTicks > Now().mTicks; }

	constexpr DateTime GetDate() const { return DateTime(mTicks - (mTicks % Time::TicksPerDay)); }
	constexpr void GetDate(I32& year, I32& month, I32& day) const
	{
		// Based on FORTRAN code in:
		// Fliegel, H. F. and van Flandern, T. C.,
		// Communications of the ACM, Vol. 11, No. 10 (October 1968).

		I32 i = 0, j = 0, k = 0, l = 0, n = 0;

		l = static_cast<I32>(ToJulianDay() + 0.5) + 68569;
		n = 4 * l / 146097;
		l = l - (146097 * n + 3) / 4;
		i = 4000 * (l + 1) / 1461001;
		l = l - 1461 * i / 4 + 31;
		j = 80 * l / 2447;
		k = l - 2447 * j / 80;
		l = j / 11;
		j = j + 2 - 12 * l;
		i = 100 * (n - 49) + i + l;

		year = i;
		month = j;
		day = k;
	}
	constexpr I32 GetYear() const { I32 y = 0, m = 0, d = 0; GetDate(y, m, d); return y; }
	constexpr I32 GetMonth() const { I32 y = 0, m = 0, d = 0; GetDate(y, m, d); return m; }
	constexpr I32 GetDay() const { I32 y = 0, m = 0, d = 0; GetDate(y, m, d); return d; }
	constexpr Month GetMonthOfTheYear() const { return static_cast<DateTime::Month>(GetMonth()); }
	constexpr Day GetDayOfTheWeek() const { return static_cast<DateTime::Day>(1 + (mTicks / Time::TicksPerDay) % 7); } // January 1, 0001 was a Monday
	constexpr I32 GetDayOfTheYear() const 
	{ 
		I32 y = 0, m = 0, d = 0; 
		GetDate(y, m, d); 
		for (I32 month = 1; month < m; ++month)
		{
			d += DaysInMonth(y, month);
		}
		return d;
	}
	constexpr F64 ToJulianDay() const { return static_cast<F64>(1721425.5 + mTicks / Time::TicksPerDay); }
	constexpr I64 ToUnixTimestamp() const { return (mTicks - UnixDate().mTicks) / Time::TicksPerSecond; }

	constexpr Time GetTime() const { return Time(mTicks % Time::TicksPerDay); }
	constexpr void GetTime(I32& hour, I32& minute) const { hour = GetHour(); minute = GetMinute(); }
	constexpr void GetTime(I32& hour, I32& minute, I32& second) const { GetTime(hour, minute); second = GetSecond(); }
	constexpr I32 GetHour() const { return static_cast<I32>((mTicks / Time::TicksPerHour) % 24); }
	constexpr I32 GetMinute() const { return static_cast<I32>((mTicks / Time::TicksPerMinute) % 60); }
	constexpr I32 GetSecond() const { return static_cast<I32>((mTicks / Time::TicksPerSecond) % 60); }
	constexpr I64 GetTicks() const { return mTicks; }
	constexpr bool IsAfternoon() const { return GetHour() >= 12; }
	constexpr bool IsMorning() const { return GetHour() < 12; }

	static constexpr bool Validate(I32 year, I32 month, I32 day, I32 hour = 0, I32 minute = 0, I32 second = 0)
	{
		return (year >= 1) && (year <= 9999) && (month >= 1) && (month <= 12) && (day >= 1) && (day <= DaysInMonth(year, month)) &&
			(hour >= 0) && (hour <= 23) && (minute >= 0) && (minute <= 59) && (second >= 0) && (second <= 59);
	}

	static constexpr I32 DaysInYear(I32 year) { return IsLeapYear(year) ? 366 : 365; }
	static constexpr I32 DaysInMonth(I32 year, I32 month)
	{
		constexpr I32 daysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		return ((month > 2) && IsLeapYear(year)) ? 29 : daysPerMonth[month];
	}
	static constexpr bool IsLeapYear(I32 year) { return ((year % 4) == 0) ? (((year % 100) != 0) || ((year % 400) == 0)) : false; }

	static DateTime Now();
	static DateTime Today();

	static constexpr DateTime UnixDate() { return DateTime(1970, 1, 1); }
	static const DateTime ProgramStart;

private:
	I64 mTicks;
};

} // namespace en

#endif // ENLIVE_MODULE_PLATFORM