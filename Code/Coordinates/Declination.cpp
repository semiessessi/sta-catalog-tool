#include "Declination.h"

#include "Maths/Maths.h"

namespace SCT
{

Declination::Declination(
	const int degrees,
	const int minutes,
	const double seconds)
: m_seconds(seconds)
, m_degrees(degrees)
, m_minutes(minutes)
{

}

double Declination::ToDegrees() const
{
	return static_cast<double>(m_degrees)
		+ static_cast<double>(m_minutes) * (1.0 / 60.0)
		+ m_seconds * (1.0 / (60.0 * 60.0));
}

double Declination::ToRadians() const
{
	return DegreesToRadians(ToDegrees());
}

Declination Declination::FromDegrees(const double degrees)
{
	const double absDegrees = abs(degrees);
	const int wholeDegrees = static_cast<int>(floor(absDegrees));
	const double minutes = (absDegrees - static_cast<double>(wholeDegrees)) * 60.0;
	const int wholeMinutes = static_cast<int>(floor(minutes));
	const double seconds = (minutes - static_cast<double>(wholeMinutes)) * 60.0;
	return Declination((degrees < 0) ? - wholeDegrees : wholeDegrees, wholeMinutes, seconds);
}

}
