#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Maths/Astronomy.h"
#include "Maths/Maths.h"
#include "Star/Star.h"

#include <cstdlib>

namespace SCT
{

inline double GetGMSTSeconds(const double julianDay)
{
	// convert Julian Day to centuries since J2000.0
	const double t = ToJulianCenturiesSinceJ2000(julianDay);

	// terms for GMST expression
	const double terms[] =
	{
		24110.54841,
		8640184.812866,
		0.093104,
		-6.2e-6
	};

	// evaluate GSMT0 using the terms
	// based on this expression for GMST in seconds
	// http://www.astro.sunysb.edu/metchev/AST443/times.html
	const double jMidnight = julianDay - fmod(julianDay - 0.5, 1.0);
	const double tMidnight = ToJulianCenturiesSinceJ2000(jMidnight);
	const double gmst0 = EvaluatePolynomial(terms, 3, tMidnight);

	// this was found on stack exchange...
	const double wTerms[] =
	{
		1.00273790935,
		5.9e-11
	};

	const double wStar = EvaluatePolynomial(wTerms, 1, t);

	// get the time of day in seconds from midnight
	const double utcSeconds = (julianDay - jMidnight) * 24 * 3600;

	// calculate mod number of seconds in the day...
	return fmod(gmst0 + wStar * utcSeconds, 86400.0);
}

// 10th order calculation of obliquity of the ecliptic
// claims to be valid over 10,000 years
double CalculateMeanObliquityOfEcliptic(const double julianDay)
{
	// this is based on J. Meeus, Astronomical Algorithms, p. 135

	// convert Julian Day to centuries since J2000.0
	const double t = ToJulianCenturiesSinceJ2000(julianDay);

	// terms for the obliquity evaluation
	//const terms =
	//[
	//	84381.448, // J2000 obliquity
	//	-46.8150,
	//	-0.00059,
	//	0.001813
	//];
	const double terms100[] =
	{
		84381.448, // J2000 obliquity
		-4680.93,
		-1.55,
		1999.25,
		-51.38,
		-249.67,
		-39.05,
		7.12,
		27.87,
		5.79,
		2.45
	};

	// so clamping is continuous, but a more accurate approach is to use
	// the long-term mean obliquity and ignore the small differences...
	const double arcseconds = EvaluatePolynomial(terms100, 10, Clamp(t / 100, -1.0, 1.0));
	const double degrees = arcseconds / 3600.0;
	return DegreesToRadians(degrees);
}


double SolarMeanAnomaly(const double julianDay)
{
	const double t = ToJulianCenturiesSinceJ2000(julianDay);
	// mean anomaly of the Sun
	// (the polynomial is evaluated using Horner's scheme)
	// Meeus p. 308
	return 357.5291092 + t * (35999.0502909 + t * (-0.0001536 + t / 24490000.0));
	// from Meeus also but lower precision...?
	//return 357.52910 + t * (35999.0530 + t * (-0.0001559 + t * -0.00000048));
}

// this produces altitude an azimuth with corrections around J2000.0
std::pair<double, double> CalculateAltitudeAzimuth(
	const double julianDay,
	const double /*RightAscension*/ raJ2000,
	const double /*Declination*/ decJ2000,
	const double latitude,
	const double longitude)
{
	const double obliquity = CalculateMeanObliquityOfEcliptic(julianDay);
	// SE - TODO: fix this, we skip because we dont need that accuracy really.
	const double nutation = 0.0;// calculateNutationLongitude(julianDay);

	// convert latitude and longitude to radians
	const double latRad = DegreesToRadians(latitude);
	const double lonRad = DegreesToRadians(longitude);

	// convert right ascension (RA) and declination (Dec) to radians
	const double raJ2000Rad = DegreesToRadians(raJ2000);
	const double decJ2000Rad = DegreesToRadians(decJ2000);

	// then precession to the current epoch...
	const auto raDec = RADecJ2000ToCurrentEpochRadians(julianDay, raJ2000Rad, decJ2000Rad);
	const double raRad = raDec.first;
	const double decRad = raDec.second;

	// calculate the Greenwich mean sidereal time (GMST)
	// put it into hours...
	double gmst = fmod(GetGMSTSeconds(julianDay) / 3600.0, 24.0);
	if (gmst < 0)
	{
		gmst += 24;
	}

	// then degrees... then radians
	gmst = DegreesToRadians(15 * gmst);

	// apply nutation and obliquity correction to right ascension and declination
	const double raRadCorrected = raRad + nutation;
	const double decRadCorrected = decRad + obliquity
		- DegreesToRadians(84381.448 / 3600); // change since J2000.0

	// calculate the hour angle
	const double hourAngleRad = gmst - lonRad - raRadCorrected;

	// calculate the sine and cosine of the latitude and declination
	const double sinLat = sin(latRad);
	const double cosLat = cos(latRad);
	const double sinDec = sin(decRadCorrected);
	const double cosDec = cos(decRadCorrected);
	const double tanDec = sinDec / cosDec;
	// ... and the hour angle
	const double sinH = sin(hourAngleRad);
	const double cosH = cos(hourAngleRad);

	// calculate the altitude and azimuth
	const double altitude = asin(
		sinLat * sinDec + cosLat * cosDec * cosH);
	double  azimuth = atan2(
		sinH,
		cosH * sinLat - tanDec * cosLat);
	if (azimuth < 0)
	{
		azimuth += 2 * PI;
	}

	// SE - NOTE: we leave it in radians
	return std::make_pair(altitude, azimuth);
}

std::pair<double, double> CalculateSunRaDec(const double julianDay)
{
	// this follows the process on Meeus p.152
	// convert Julian Day to centuries since J2000.0
	const double t = ToJulianCenturiesSinceJ2000(julianDay);

	// mean longitude of the Sun
	const double L = 280.46646 + t * (36000.76983 + t * 0.0003032);

	// mean anomaly of the Sun
	const double M = SolarMeanAnomaly(julianDay);

	// eccentricity of the Earth's orbit
	//const e = 0.016708634 + t * (-0.000042037 - t * 0.0000001267);

	//const Lrad = degreesToRadians(L);
	const double Mrad = DegreesToRadians(M);

	// equation of center
	const double C = ((1.914602 + t * (-0.004817 + t * -0.000014)) * sin(Mrad)) +
		((0.019993 - 0.000101 * t) * sin(2 * Mrad)) +
		(0.000289 * sin(3 * Mrad));

	// true longitude of the Sun
	const double trueLongitude = L + C;

	// apparent longitude of the Sun, correcting for nutation and abberation
	const double omega = 125.04 - 1934.136 * t;
	const double omegaRad = DegreesToRadians(omega);
	const double lambda = trueLongitude - 0.00569 - 0.00478 * sin(omegaRad);

	// (mean) obliquity of the ecliptic, with correction...
	const double epsilonRad = CalculateMeanObliquityOfEcliptic(julianDay)
		+ DegreesToRadians(0.00256) * cos(omegaRad);

	// calculate the Sun's right ascension (RA)
	const double lambdaRad = DegreesToRadians(lambda);
	double RA = atan2(
		cos(epsilonRad) * sin(lambdaRad),
		cos(lambdaRad));
	if (RA < 0)
	{
		RA += PI * 2;
	}

	// calculate the Sun's declination (Dec)
	const double dec = asin(sin(epsilonRad) * sin(lambdaRad));

	return std::make_pair(RA, dec);
}

// calculate position of Sun in the for a given time and place
std::pair<double, double> EvaluateSunCoordinates(
	const double julianDay, const double latitude, const double longitude)
{
	const auto sunRADec = CalculateSunRaDec(julianDay);
	return //AtmosphericAdjustment(
		CalculateAltitudeAzimuth(
			julianDay,
			RadiansToDegrees(sunRADec.first),
			RadiansToDegrees(sunRADec.second),
			latitude,
			longitude);//);
}

//const double sunRiseAngle = degreesToRadians(-18 / 60);

double CalculateNextSunrise(
	const double julianDay,
	const double latitude,
	const double longitude,
	const double angle)
{
	// try to find a point where the sun is below the horizon to start...
	// work out its position at each hour for the next 24 hours to start.
	// it is simple minded but allows a binary search to complete the problem.
	// ... in most cases

	double hoursSunAlt[96];
	hoursSunAlt[0] = EvaluateSunCoordinates(julianDay, latitude, longitude).first;
	double lastSunAlt = hoursSunAlt[0];
	double riseHour = -1;
	double timeStep = 1.0 / 24.0;
	for (int i = 1; i < 96; ++i)
	{
		hoursSunAlt[i] = EvaluateSunCoordinates(
			julianDay + i * timeStep, latitude, longitude).first;
		if ((riseHour < 0) && (hoursSunAlt[i] > angle) && (lastSunAlt < angle))
		{
			riseHour = i;
			break;
		}
		lastSunAlt = hoursSunAlt[i];
	}

	// binary search it a few steps...
	double lower = riseHour - 1;
	double upper = riseHour;
	double lowerCoords = EvaluateSunCoordinates(
		julianDay + lower * timeStep, latitude, longitude).first;
	double upperCoords = EvaluateSunCoordinates(
		julianDay + upper * timeStep, latitude, longitude).first;
	for (int i = 1; i < 16; ++i)
	{
		double newValue = (lower + upper) / 2.0;
		double newCoords = EvaluateSunCoordinates(
			julianDay + newValue * timeStep, latitude, longitude).first;
		if (newCoords < angle)
		{
			lower = newValue;
			lowerCoords = newCoords;
		}
		else
		{
			upper = newValue;
			upperCoords = newCoords;
		}

	}

	return (lower + upper) / 2.0;
}

double CalculateNextSunset(
	const double julianDay,
	const double latitude,
	const double longitude,
	const double angle)
	{
		// try to find a point where the sun is below the horizon to start...
		// work out its position at each hour for the next 24 hours to start.
		// it is simple minded but allows a binary search to complete the problem.
		// ... in most cases

		double hoursSunAlt[96];
		hoursSunAlt[0] = EvaluateSunCoordinates(julianDay, latitude, longitude).first;
		double lastSunAlt = hoursSunAlt[0];
		double riseHour = -1;
		double timeStep = 1.0 / 24.0;
		for (int i = 1; i < 96; ++i)
		{
			hoursSunAlt[i] = EvaluateSunCoordinates(
				julianDay + i * timeStep, latitude, longitude).first;
			if ((riseHour < 0) && (hoursSunAlt[i] < angle) && (lastSunAlt > angle))
			{
				riseHour = i;
				break;
			}
			lastSunAlt = hoursSunAlt[i];
		}

		// binary search it a few steps...
		double lower = riseHour - 1;
		double upper = riseHour;
		double lowerCoords = EvaluateSunCoordinates(
			julianDay + lower * timeStep, latitude, longitude).first;
		double upperCoords = EvaluateSunCoordinates(
			julianDay + upper * timeStep, latitude, longitude).first;
		for (int i = 1; i < 16; ++i)
		{
			double newValue = (lower + upper) / 2.0;
			double newCoords = EvaluateSunCoordinates(
				julianDay + newValue * timeStep, latitude, longitude).first;
			if (newCoords > angle)
			{
				lower = newValue;
				lowerCoords = newCoords;
			}
			else
			{
				upper = newValue;
				upperCoords = newCoords;
			}

		}

		return (lower + upper) / 2.0;
}

void FindPotentialDecans()
{
	std::vector<int> daysDown;
	std::vector<SCT::StarSystem> potentials;
	std::vector<SCT::StarSystem> close;
	std::vector<int> closeRiseDays;
	std::vector<int> potentialRiseDays;
	Catalog::ForEachStar([&](SCT::StarSystem& star)
	{
		static const double kTestLatitude = 29.9792;
		static const double kTestLongitude = -31.1342;
		static const double kArcusVisionisSopdet = DegreesToRadians(8);
		static const double kSopdetRiseAltitude = DegreesToRadians(2.5); // TODO: choose this?

		// look at the 365 days around 4.2kya
		static const double kJ2000 = 2451545.0;
		int count = 0;
		int riseDay = -1;
		int lastRisen = false;
		for (int i = 0; i < 365; ++i)
		{
			const double startJD = kJ2000 - 4200.0 * 365.25 + i;
			const double sunsetTime = CalculateNextSunset(
				startJD, kTestLatitude, kTestLongitude, -kArcusVisionisSopdet + kSopdetRiseAltitude) / 24.0;
			const double sunriseTime = CalculateNextSunrise(
				sunsetTime + startJD, kTestLatitude, kTestLongitude, -kArcusVisionisSopdet + kSopdetRiseAltitude) / 24.0;
			const double riseAltitude = CalculateAltitudeAzimuth(
				startJD + sunriseTime,
				star.GetRA().ToDegrees(), star.GetDec().ToDegrees(),
				kTestLatitude, kTestLongitude).first;
			const bool risen = riseAltitude > kSopdetRiseAltitude;
			if (!risen)
			{
				const double step = (sunriseTime - sunsetTime) / 10.1;
				for (double d = sunsetTime; d < sunriseTime; d += step)
				{
					const double altitude = CalculateAltitudeAzimuth(
						startJD + d,
						star.GetRA().ToDegrees(), star.GetDec().ToDegrees(),
						kTestLatitude, kTestLongitude).first;
					if (altitude > kSopdetRiseAltitude)
					{
						++count;
						break;
					}
				}
			}
			else
			{
				++count;
			}

			if ((lastRisen == false) && (risen == true))
			{
				riseDay = i;
			}

			lastRisen = risen;
		}

		if (star.GetHR() == 2491)
		{
			printf("\rSopdet (2491): %d, %d\n", 365 - count, riseDay);
		}
		else if (star.GetHR() == 2943)
		{
			printf("\rProcyon (2943): %d, %d\n", 365 - count, riseDay);
		}

		if ((daysDown.size() & 31) == 0)
		{
			printf("\r%d / %d", (int)(daysDown.size()), (int)(Catalog::Count()));
		}

		daysDown.push_back(365 - count);
		if (((365 - count) > 70)
			&& ((365 - count) < 245))
		{
			//printf("Found potential HR %d\n", star.GetHR());
			int testDay = 10;
			// this kinda makes a nicer list.
			if (riseDay < 272)//(riseDay < 282)
			{
				testDay = (riseDay - 5) % 10;
			}
			else
			{
				testDay = (riseDay + 0) % 10;
			}
			if ((testDay < 5)
				&& (testDay > -5))
			{
				//printf("... and is a close match!\n");
				close.push_back(star);
				closeRiseDays.push_back(riseDay);
			}

			potentials.push_back(star);
			potentialRiseDays.push_back(riseDay);
		}
	});

	printf("\rPotential decan stars: %d\n", (int)potentials.size());
	printf("Potential close geometric matches: %d\n", (int)close.size());

	int potentialAboveMag3 = 0;
	for (size_t i = 0; i < potentials.size(); ++i)
	{
		if (potentials[i].GetVisualMagnitude() < 4.2f)
		{
			++potentialAboveMag3;
		}
	}

	int closeAboveMag3 = 0;
	for (size_t i = 0; i < close.size(); ++i)
	{
		if (close[i].GetVisualMagnitude() < 3.5f)
		{
			++closeAboveMag3;
		}
	}

	printf("Potential bright decan stars: %d\n", potentialAboveMag3);
	for (size_t i = 0; i < potentials.size(); ++i)
	{
		if (potentials[i].GetVisualMagnitude() < 4.2f)
		{
			printf("%s mag %.2f rise day %d\n", potentials[i].GetNameString().c_str(), potentials[i].GetVisualMagnitude(), potentialRiseDays[i]);
		}
	}
	
	printf("Potential close bright geometric matches: %d\n", closeAboveMag3);
	for (size_t i = 0; i < close.size(); ++i)
	{
		if (close[i].GetVisualMagnitude() < 3.5f)
		{
			printf("%s mag %.2f rise day %d\n", close[i].GetNameString().c_str(), close[i].GetVisualMagnitude(), closeRiseDays[i]);
		}
	}

	return;
}

}
