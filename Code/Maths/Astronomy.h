#pragma once

#ifndef ASTRONOMY_H
#define ASTRONOMY_H

#include "Maths.h"

namespace SCT
{

static const double kJ2000 = 2451545.0;

static inline double ToJulianCenturies(const double days)
{
	return days / 36525;
}

static inline double ToJulianCenturiesSinceJ2000(const double julianDay)
{
	return ToJulianCenturies(julianDay - kJ2000);
}

static inline std::pair<double, double> RADecJ2000ToCurrentEpochRadians(const double julianDay, const double raJ2000, const double decJ2000)
{
	// TODO: https://ui.adsabs.harvard.edu/abs/2011A%26A...534A..22V/abstract
	// from Meeus p 134
	// convert Julian Day to centuries since J2000.0
	const double  t = ToJulianCenturiesSinceJ2000(julianDay);

	// note these are only valid for J2000.0, the coefficients vary for different epochs
	const double zeta = DegreesToRadians(
		t * (2306.2181 + t * (0.30188 + t * 0.017998)) / 3600);
	const double z = DegreesToRadians(
		t * (2306.2181 + t * (1.09468 + t * 0.018203)) / 3600);
	const double theta = DegreesToRadians(
		t * (2004.3109 + t * (0.42665 + t * 0.041833)) / 3600);

	const double cosDecJ2000 = cos(decJ2000);
	const double sinDecJ2000 = sin(decJ2000);
	const double cosAdjustedRA = cos(raJ2000 + zeta);
	const double sinAdjustedRA = sin(raJ2000 + zeta);
	const double cosTheta = cos(theta);
	const double sinTheta = sin(theta);
	const double A = cosDecJ2000 * sinAdjustedRA;
	const double B = cosTheta * cosDecJ2000 * cosAdjustedRA - sinTheta * sinDecJ2000;
	const double C = sinTheta * cosDecJ2000 * cosAdjustedRA + cosTheta * sinDecJ2000;

	const double ra = atan2(A, B) + z;
	const double dec = asin(C);
	return std::make_pair(ra, dec);
}

static inline std::pair<double, double> RADecCurrentEpochToJ2000Radians(const double julianDay, const double ra, const double dec)
{
	// TODO: https://ui.adsabs.harvard.edu/abs/2011A%26A...534A..22V/abstract
	// from Meeus p 134
	// convert Julian Day to centuries since J2000.0
	const double t = ToJulianCenturies(kJ2000 - julianDay);
	const double T = ToJulianCenturiesSinceJ2000(julianDay);

	// note these are only valid for J2000.0, the coefficients vary for different epochs
	const double zeta = DegreesToRadians(
		t * ((2306.2181 + T * (1.39656 - T * 0.000139))
			+ t * ((0.30188 -  T * 0.000344)
				+ t * 0.017998)) / 3600);
	const double z = DegreesToRadians(
		t * ((2306.2181 + T * (1.39656 - T * 0.000139))
			+ t * ((1.09468 + T * 0.000066)
				+ t * 0.018203)) / 3600);
	const double theta = DegreesToRadians(
		t * ((2004.3109 - T * (0.85330 + T * 0.000217))
			+ t * ((-0.42665 - T * 0.000217)
				+ t * 0.041833)) / 3600);

	const double cosDecEpoch = cos(dec);
	const double sinDecEpoch = sin(dec);
	const double cosAdjustedRA = cos(ra + zeta);
	const double sinAdjustedRA = sin(ra + zeta);
	const double cosTheta = cos(theta);
	const double sinTheta = sin(theta);
	const double A = cosDecEpoch * sinAdjustedRA;
	const double B = cosTheta * cosDecEpoch * cosAdjustedRA - sinTheta * sinDecEpoch;
	const double C = sinTheta * cosDecEpoch * cosAdjustedRA + cosTheta * sinDecEpoch;

	const double raJ2000 = atan2(A, B) + z;
	const double decJ2000 = asin(C);
	return std::make_pair(raJ2000, decJ2000);
}

static inline std::pair<double, double> RADecJ2000ToCurrentEpochDegrees(const double julianDay, const double raJ2000, const double decJ2000)
{
	const double decRadians = DegreesToRadians(decJ2000);
	const double raRadians = DegreesToRadians(raJ2000);
	auto result = RADecJ2000ToCurrentEpochRadians(julianDay, raRadians, decRadians);
	return std::make_pair(RadiansToDegrees(result.first), RadiansToDegrees(result.second));
}

static inline std::pair<double, double> RADecCurrentEpochToJ2000Degrees(const double julianDay, const double ra, const double dec)
{
	const double decRadians = DegreesToRadians(dec);
	const double raRadians = DegreesToRadians(ra);
	auto result = RADecCurrentEpochToJ2000Radians(julianDay, raRadians, decRadians);
	return std::make_pair(RadiansToDegrees(result.first), RadiansToDegrees(result.second));
}

}

#endif
