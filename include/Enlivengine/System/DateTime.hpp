#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Time.hpp>

#include <string>

// TODO : Check C++20 to improve this !

namespace en
{

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
		DateTime(I64 ticks);
		DateTime(I32 year, I32 month, I32 day, I32 hour = 0, I32 minute = 0, I32 second = 0, I32 millisecond = 0);

		DateTime& set(I64 ticks);
		DateTime& set(I32 year, I32 month, I32 day, I32 hour = 0, I32 minute = 0, I32 second = 0, I32 millisecond = 0);
		DateTime& fromJulianDay(double julianDay);
		DateTime& fromUnixTimestamp(I64 unixTime);

		DateTime operator+(const Time& time) const;
		DateTime& operator+=(const Time& time);
		Time operator-(const DateTime& date) const;
		DateTime operator-(const Time& time) const;
		DateTime& operator-=(const Time& time);

		bool operator==(const DateTime& date) const;
		bool operator!=(const DateTime& date) const;
		bool operator>(const DateTime& date) const;
		bool operator>=(const DateTime& date) const;
		bool operator<(const DateTime& date) const;
		bool operator<=(const DateTime& date) const;
		bool isPast() const;
		bool isFuture() const;

		DateTime getDate() const;
		void getDate(I32& year, I32& month, I32& day) const;
		I32 getYear() const;
		I32 getMonth() const;
		I32 getDay() const;
		Month getMonthOfTheYear() const;
		Day getDayOfTheWeek() const;
		I32 getDayOfTheYear() const;
		double toJulianDay() const;
		I64 toUnixTimestamp() const;

		Time getTime() const;
		void getTime(I32& hour, I32& minute) const;
		void getTime(I32& hour, I32& minute, I32& second) const;
		void getTime(I32& hour, I32& minute, I32& second, I32& millisecond) const;
		I32 getHour() const;
		I32 getHour12() const;
		I32 getMinute() const;
		I32 getSecond() const;
		I32 getMillisecond() const;
		I64 getTicks() const;
		bool isAfternoon() const;
		bool isMorning() const;

		static bool validate(I32 year, I32 month, I32 day, I32 hour = 0, I32 minute = 0, I32 second = 0, I32 millisecond = 0);

		static I32 daysInYear(I32 year);
		static I32 daysInMonth(I32 year, I32 month);
		static bool isLeapYear(I32 year);

		static DateTime now();
		static DateTime today();

	private:
		I64 mTicks;
};

} // namespace en