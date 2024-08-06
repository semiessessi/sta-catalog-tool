#include "RightAscension.h"

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
	return static_cast<double>(m_hours) * (360.0 / 24.0)
		+ static_cast<double>(m_minutes) * (1.0 / 60.0)
		+ m_seconds * (1.0 / (60.0 * 60.0));
}

}
