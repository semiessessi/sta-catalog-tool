#pragma once 

#ifndef RIGHT_ASCENSION_H
#define RIGHT_ASCENSION_H

namespace SCT
{

class RightAscension
{
public:

	RightAscension(const int hours, const int minutes = 0, const double seconds = 0.0);

	double ToDegrees() const;

private:

	double m_seconds = 0.0;
	unsigned char m_hours = 0;
	unsigned char m_minutes = 0;
};

}

#endif
