#pragma once 

#ifndef STAR_H
#define STAR_H

#include "StarCatalog.h"

#include "Coordinates/Declination.h"
#include "Coordinates/RightAscension.h"

#include <cstdint>
#include <string>

namespace SCT
{

class StarSystem
{
public:

	StarSystem(const StarCatalog catalog);
	StarSystem(const StarCatalog catalog, const uint64_t sourceCatalogIndex);

	void SetHR(const int hr) { m_HR = hr; }
	void SetHD(const int hd) { m_HD = hd; }
	void SetHip(const int hip) { m_Hip = hip; }
	void SetFlamsteed(const int flamsteed) { m_flamsteed = flamsteed; }

	void SetVisualMagnitude(const float magnitude) { m_visualMagnitude = magnitude; }
	void SetBMinusV(const float magnitude) { m_BminusVMagnitude = magnitude; }

	void SetRA(const int hours, const int minutes = 0, const double seconds = 0.0);
	void SetDeclination(const int degrees, const int minutes = 0, const double seconds = 0.0);

	void SetNameString(const std::string& name) { m_crappyNameString = name; }

	const RightAscension& GetRA() const { return m_RA; }
	const Declination& GetDec() const { return m_Dec; }

	int GetHip() const { return m_Hip; }
	int GetHD() const { return m_HD; }
	int GetHR() const { return m_HR; }

	float GetVisualMagnitude() const { return m_visualMagnitude; }
	float GetBMinusV() const { return m_BminusVMagnitude; }

	std::string GetNameString() const;

private:

	std::string m_crappyNameString;
	RightAscension m_RA;
	Declination m_Dec;
	float m_visualMagnitude = 50.0f;
	float m_BminusVMagnitude = 50.0f;
	unsigned int m_Hip = 0;
	unsigned int m_HD = 0;
	unsigned short m_HR = 0;
	unsigned char m_flamsteed = 0;
	StarCatalog m_catalog;
};

}

#endif
