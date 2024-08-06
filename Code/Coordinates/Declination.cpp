#include "Declination.h"

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

}
