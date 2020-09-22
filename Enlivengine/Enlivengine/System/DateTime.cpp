#include <Enlivengine/System/DateTime.hpp>

#include <ctime>

namespace en
{

static const DateTime ProgramStart = DateTime::Now();

DateTime::DateTime()
	: mTicks(Now().mTicks)
{
}

DateTime DateTime::Now()
{
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo = localtime(&rawtime);

	return DateTime(
		timeinfo->tm_year + 1900, 
		timeinfo->tm_mon + 1, // months since January 0-11
		timeinfo->tm_mday, // day of the month 1-31
		timeinfo->tm_hour, // hours since midnight 0-23
		timeinfo->tm_min, // minutes after the hour 0-59
		timeinfo->tm_sec // seconds after the minute 0-59
	);
}

DateTime DateTime::Today()
{
	return Now().GetDate();
}

} // namespace en
