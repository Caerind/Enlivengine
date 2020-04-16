#include <Enlivengine/System/DateTime.hpp>

#include <Enlivengine/System/Assert.hpp>
#include <ctime>

namespace en
{

DateTime::DateTime()
	: mTicks(now().mTicks)
{
}

DateTime::DateTime(I64 ticks)
	: mTicks(ticks)
{
}

DateTime::DateTime(I32 year, I32 month, I32 day, I32 hour, I32 minute, I32 second, I32 millisecond)
{
	set(year, month, day, hour, minute, second, millisecond);
}

DateTime& DateTime::set(I64 ticks)
{
	mTicks = ticks;
	return *this;
}

DateTime& DateTime::set(I32 year, I32 month, I32 day, I32 hour, I32 minute, I32 second, I32 millisecond)
{
	assert(validate(year, month, day, hour, minute, second, millisecond));
	static const I32 daysToMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	I32 days = 0;
	if ((month > 2) && isLeapYear(year))
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
	mTicks = days * Time::TicksPerDay + hour * Time::TicksPerHour + minute * Time::TicksPerMinute + second * Time::TicksPerSecond + millisecond * Time::TicksPerMillisecond;
	return *this;
}

DateTime& DateTime::fromJulianDay(double julianDay)
{
	mTicks = (I64)((julianDay - 1721425.5) * Time::TicksPerDay);
	return *this;
}

DateTime& DateTime::fromUnixTimestamp(I64 unixTime)
{
	static const DateTime unixDate(1970, 1, 1);
	mTicks = unixDate.mTicks + unixTime * Time::TicksPerSecond;
	return *this;
}

DateTime DateTime::operator+(const Time& time) const
{
	return DateTime(mTicks + time.getTicks());
}

DateTime& DateTime::operator+=(const Time& time)
{
	mTicks += time.getTicks();
	return *this;
}

Time DateTime::operator-(const DateTime& date) const
{
	return Time(mTicks - date.mTicks);
}

DateTime DateTime::operator-(const Time& time) const
{
	return DateTime(mTicks - time.getTicks());
}

DateTime& DateTime::operator-=(const Time& time)
{
	mTicks -= time.getTicks();
	return *this;
}

bool DateTime::operator==(const DateTime& date) const
{
	return mTicks == date.mTicks;
}

bool DateTime::operator!=(const DateTime& date) const
{
	return mTicks != date.mTicks;
}

bool DateTime::operator>(const DateTime& date) const
{
	return mTicks > date.mTicks;
}

bool DateTime::operator>=(const DateTime& date) const
{
	return mTicks >= date.mTicks;
}

bool DateTime::operator<(const DateTime& date) const
{
	return mTicks < date.mTicks;
}

bool DateTime::operator<=(const DateTime& date) const
{
	return mTicks <= date.mTicks;
}

bool DateTime::isPast() const
{
	return mTicks < now().mTicks;
}

bool DateTime::isFuture() const
{
	return mTicks > now().mTicks;
}

DateTime DateTime::getDate() const
{
	return DateTime(mTicks - (mTicks % Time::TicksPerDay));
}

void DateTime::getDate(I32& year, I32& month, I32& day) const
{
	// Based on FORTRAN code in:
	// Fliegel, H. F. and van Flandern, T. C.,
	// Communications of the ACM, Vol. 11, No. 10 (October 1968).

	I32 i, j, k, l, n;

	l = (I32)(toJulianDay() + 0.5) + 68569;
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

I32 DateTime::getYear() const
{
	I32 y, m, d;
	getDate(y, m, d);
	return y;
}

I32 DateTime::getMonth() const
{
	I32 y, m, d;
	getDate(y, m, d);
	return m;
}

I32 DateTime::getDay() const
{
	I32 y, m, d;
	getDate(y, m, d);
	return d;
}

DateTime::Month DateTime::getMonthOfTheYear() const
{
	return static_cast<DateTime::Month>(getMonth());
}

DateTime::Day DateTime::getDayOfTheWeek() const
{ 
	return static_cast<DateTime::Day>(1 + (mTicks / Time::TicksPerDay) % 7); // January 1, 0001 was a Monday
}

I32 DateTime::getDayOfTheYear() const
{
	I32 y, m, d;
	getDate(y, m, d);
	for (I32 month = 1; month < m; month++)
	{
		d += daysInMonth(y, month);
	}
	return d;
}

double DateTime::toJulianDay() const
{
	return (double)(1721425.5 + mTicks / Time::TicksPerDay);
}

I64 DateTime::toUnixTimestamp() const
{
	static const DateTime unixDate(1970, 1, 1);
	return (mTicks - unixDate.mTicks) / Time::TicksPerSecond;
}

Time DateTime::getTime() const
{
	return Time(mTicks % Time::TicksPerDay);
}

void DateTime::getTime(I32& hour, I32& minute) const
{
	hour = getHour();
	minute = getMinute();
}

void DateTime::getTime(I32& hour, I32& minute, I32& second) const
{
	hour = getHour();
	minute = getMinute();
	second = getSecond();
}

void DateTime::getTime(I32& hour, I32& minute, I32& second, I32& millisecond) const
{
	hour = getHour();
	minute = getMinute();
	second = getSecond();
	millisecond = getMillisecond();
}

I32 DateTime::getHour() const
{
	return (I32)((mTicks / Time::TicksPerHour) % 24);
}

I32 DateTime::getHour12() const
{
	I32 h = getHour();
	if (h < 1)
	{
		return 12;
	}
	if (h > 12)
	{
		return (h - 12);
	}
	return h;
}

I32 DateTime::getMinute() const
{
	return (I32)((mTicks / Time::TicksPerMinute) % 60);
}

I32 DateTime::getSecond() const
{
	return (I32)((mTicks / Time::TicksPerSecond) % 60);
}

I32 DateTime::getMillisecond() const
{
	return (I32)((mTicks / Time::TicksPerMillisecond) % 1000);
}

I64 DateTime::getTicks() const
{
	return mTicks;
}

bool DateTime::isAfternoon() const
{
	return getHour() >= 12;
}

bool DateTime::isMorning() const
{
	return getHour() < 12;
}

bool DateTime::validate(I32 year, I32 month, I32 day, I32 hour, I32 minute, I32 second, I32 millisecond)
{
	return (year >= 1) && (year <= 9999) && (month >= 1) && (month <= 12) && (day >= 1) && (day <= daysInMonth(year, month)) &&
		(hour >= 0) && (hour <= 23) && (minute >= 0) && (minute <= 59) && (second >= 0) && (second <= 59) && (millisecond >= 0) && (millisecond <= 999);
}

I32 DateTime::daysInYear(I32 year)
{
	if (isLeapYear(year))
	{
		return 366;
	}
	return 365;
}

I32 DateTime::daysInMonth(I32 year, I32 month)
{
	assert((month >= 1) && (month <= 12));
	static const I32 daysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if ((month > 2) && isLeapYear(year))
	{
		return 29;
	}
	return daysPerMonth[month];
}

bool DateTime::isLeapYear(I32 year)
{
	if ((year % 4) == 0)
	{
		return (((year % 100) != 0) || ((year % 400) == 0));
	}
	return false;
}

DateTime DateTime::now()
{
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo = localtime(&rawtime);

	return DateTime(
		2020, // TODO : HARDCODED
		timeinfo->tm_mon + 1, // months since January 0-11
		timeinfo->tm_mday, // day of the month 1-31
		timeinfo->tm_hour, // hours since midnight 0-23
		timeinfo->tm_min, // minutes after the hour 0-59
		timeinfo->tm_sec // seconds after the minute 0-59
	);
}

DateTime DateTime::today()
{
	return now().getDate();
}

} // namespace en
