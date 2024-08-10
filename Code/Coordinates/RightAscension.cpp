#include "RightAscension.h"

#include "Maths/Maths.h"

namespace SCT
{

RightAscension::RightAscension(
	const int hours,
	const int minutes,
	const double seconds)
: m_seconds(seconds)
, m_hours(hours)
, m_minutes(minutes)
{

}

double RightAscension::ToDegrees() const
{
	return static_cast<double>(m_hours) * (15.0)
		+ static_cast<double>(m_minutes) * (1.0 / 60.0)
		+ m_seconds * (1.0 / (60.0 * 60.0));
}

double RightAscension::ToRadians() const
{
	return DegreesToRadians(ToDegrees());
}

RightAscension RightAscension::FromDegrees(const double degrees)
{
	const int wholeHours = static_cast<int>(floor(degrees / 15.0));
	const double minutes = (degrees - static_cast<double>(wholeHours)) * 60.0;
	const int wholeMinutes = static_cast<int>(floor(minutes));
	const double seconds = (minutes - static_cast<double>(wholeMinutes)) * 60.0;
	return RightAscension(wholeHours, wholeMinutes, seconds);
}

}
