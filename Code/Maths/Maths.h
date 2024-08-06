#pragma once 

#ifndef MATHS_H
#define MATHS_H

#include <cmath>

namespace SCT
{

static const double PI = 3.1415926535897932385;

static inline double DegreesToRadians(const double degrees)
{
	return (PI / 180) * degrees;
}

static inline double RadiansToDegrees(const double radians)
{
	return (180 / PI) * radians;
}

template<typename T>
static inline T EvaluatePolynomial(const T* const terms, const int order, T x)
{
	T evaluation = terms[order];
	for (int i = order - 1; i >= 0; --i)
	{
		evaluation *= x;
		evaluation += terms[i];
	}
	return evaluation;
}

template<typename T>
static inline T Clamp(const T value, const T min = (T)0.0, const T max = (T)1.0)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

}

#endif
