#pragma once 

#ifndef DECLINATION_H
#define DECLINATION_H

namespace SCT
{

class Declination
{
public:

	Declination(
		const int degrees,
		const int minutes = 0,
		const double seconds = 0.0);

	double ToDegrees() const;
	double ToRadians() const;

	static Declination FromDegrees(const double degrees);

private:

	double m_seconds = 0.0;
	char m_degrees = 0;
	unsigned char m_minutes = 0;
};

}

#endif
