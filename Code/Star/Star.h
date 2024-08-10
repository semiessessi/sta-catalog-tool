#pragma once 

#ifndef STAR_H
#define STAR_H

#include "Constellation.h"
#include "Durchmusterung.h"
#include "SpectralClass.h"
#include "StarCatalog.h"

#include "Coordinates/Declination.h"
#include "Coordinates/RightAscension.h"

#include <cstdint>
#include <string>
#include <vector>

namespace SCT
{

enum BayerGreekLetter : unsigned char
{
	None,
	Alpha,
	Beta,
	Gamma,
	Delta,
	Epsilon,
	Zeta,
	Eta,
	Theta,
	Iota,
	Kappa,
	Lambda,
	Mu,
	Nu,
	Xi,
	Omicron,
	Pi,
	Rho,
	Sigma,
	Tau,
	Upsilon,
	Phi,
	Chi,
	Psi,
	Omega
};

std::pair<BayerGreekLetter, unsigned char> ParseBayerFromName(const std::string& name);
std::string NameFromBayer(const BayerGreekLetter letter);

class StarSystem;

class ComponentStar
{
public:

	static ComponentStar FromCatalog(const StarCatalog source);

	void Merge(const StarSystem& system);

	void SetHR(const int hr) { m_HR = hr; }
	void SetHD(const int hd) { m_HD = hd; }
	void SetHip(const int hip) { m_Hip = hip; }
	void SetADS(const int ads) { m_ADS = ads; }
	void SetFK5(const int fk5) { m_FK5 = fk5; }
	void SetSAO(const int sao) { m_SAO = sao; }
	void SetDM(const Durchmusterung& dm) { m_DM = dm; }
	void SetFlamsteed(const int flamsteed) { m_flamsteed = flamsteed; }
	void SetBayer(const BayerGreekLetter letter, const int index = 0) { m_bayerLetter = letter; m_bayerIndex = index; }
	void SetSpectralClass(const SpectralClass spectralClass) { m_spectralClass = spectralClass; }
	void SetVisualMagnitude(const float magnitude) { m_visualMagnitude = magnitude; }
	void SetBMinusV(const float magnitude) { m_BminusVMagnitude = magnitude; }
	void SetComponentCode(const char code) { m_componentCode = code; }
	void SetFromSystemPrimary(const StarSystem& system);

	int GetHip() const { return m_Hip; }
	int GetHD() const { return m_HD; }
	int GetHR() const { return m_HR; }
	int GetADS() const { return m_ADS; }
	int GetFK5() const { return m_FK5; }
	int GetSAO() const { return m_SAO; }
	Durchmusterung GetDM() const { return m_DM; }
	Durchmusterung GetDMComponentless() const { return m_DM.GetComponentless(); }
	int GetFlamsteed() const { return m_flamsteed; }
	BayerGreekLetter GetBayer() const { return m_bayerLetter; }
	int GetBayerIndex() const { return m_bayerIndex; }
	char GetComponentCode() const { return m_componentCode; }

	SpectralClass GetSpectralClass() const { return m_spectralClass; }
	float GetVisualMagnitude() const { return m_visualMagnitude; }
	float GetBMinusV() const { return m_BminusVMagnitude; }

	StarCatalog GetCatalog() const { return m_catalog; }

	std::string ToDisplayTableRow(const double ra, const double dec, const Constellation constellation) const;
	std::string GetDisplayName(const Constellation constellation) const;

private:

	float m_visualMagnitude = 50.0f;
	float m_BminusVMagnitude = 50.0f;
	unsigned int m_Hip = 0;
	unsigned int m_HD = 0;
	unsigned int m_ADS = 0;
	unsigned int m_SAO = 0;
	Durchmusterung m_DM;
	unsigned short m_FK5 = 0;
	unsigned short m_HR = 0;
	SpectralClass m_spectralClass = SpectralClass::Empty;
	unsigned char m_flamsteed = 0;
	unsigned char m_bayerIndex = 0;
	BayerGreekLetter m_bayerLetter = None;
	StarCatalog m_catalog;
	char m_componentCode = 0;
};

class StarSystem
{
public:

	StarSystem(const StarCatalog catalog);
	StarSystem(const StarCatalog catalog, const uint64_t sourceCatalogIndex);

	void SetHR(const int hr) { m_components[0].SetHR(hr); }
	void SetHD(const int hd) { m_components[0].SetHD(hd); }
	void SetHip(const int hip) { m_components[0].SetHip(hip); }
	void SetADS(const int ads) { m_components[0].SetADS(ads); }
	void SetFK5(const int fk5) { m_components[0].SetFK5(fk5); }
	void SetSAO(const int sao) { m_components[0].SetSAO(sao); }
	void SetDM(const Durchmusterung& dm) { m_components[0].SetDM(dm); }
	void SetFlamsteed(const int flamsteed) { m_components[0].SetFlamsteed(flamsteed); }
	void SetBayer(const BayerGreekLetter letter, const int index = 0) { m_components[0].SetBayer(letter, index); }

	void SetConstellation(const Constellation constellation) { m_constellation = constellation; }

	void SetSpectralClass(const SpectralClass spectralClass) { m_components[0].SetSpectralClass(spectralClass); }
	void SetVisualMagnitude(const float magnitude) { m_components[0].SetVisualMagnitude(magnitude); }
	void SetBMinusV(const float magnitude) { m_components[0].SetBMinusV(magnitude); }

	void SetComponentCode(const char code) { m_components[0].SetComponentCode(code); }

	void SetRA(const RightAscension& ra) { m_RA = ra; }
	void SetRA(const int hours, const int minutes = 0, const double seconds = 0.0);
	void SetDeclination(const Declination& dec) { m_Dec = dec; }
	void SetDeclination(const int degrees, const int minutes = 0, const double seconds = 0.0);

	void SetNameString(const std::string& name) { m_crappyNameString = name; }

	const RightAscension& GetRA() const { return m_RA; }
	const Declination& GetDec() const { return m_Dec; }

	int GetHip() const { return m_components[0].GetHip(); }
	int GetHD() const { return m_components[0].GetHD(); }
	int GetHR() const { return m_components[0].GetHR(); }
	int GetADS() const { return m_components[0].GetADS(); }
	int GetFK5() const { return m_components[0].GetFK5(); }
	int GetSAO() const { return m_components[0].GetSAO(); }
	Durchmusterung GetDM() const { return m_components[0].GetDM(); }
	Durchmusterung GetDMComponentless() const { return m_components[0].GetDMComponentless(); }
	int GetFlamsteed() const { return m_components[0].GetFlamsteed(); }
	BayerGreekLetter GetBayer() const { return m_components[0].GetBayer(); }
	int GetBayerIndex() const { return m_components[0].GetBayerIndex(); }
	int GetComponentCode() const { return m_components[0].GetComponentCode(); }
	Constellation GetConstellation() const { return m_constellation; }

	StarCatalog GetCatalog() const { return m_components[0].GetCatalog(); }

	SpectralClass GetSpectralClass() const { return m_components[0].GetSpectralClass(); }
	float GetVisualMagnitude() const { return m_components[0].GetVisualMagnitude(); }
	float GetBMinusV() const { return m_components[0].GetBMinusV(); }

	std::string GetNameString() const;

	bool IsMultiple() const { return m_components.size() > 1; }

	void AccumulateNewData(const StarSystem& dataSource);

	static std::string DisplayTableHeader();
	std::string ToDisplayTableRows() const;

private:

	std::string m_crappyNameString;
	RightAscension m_RA;
	Declination m_Dec;
	Constellation m_constellation = Unknown;
	std::vector<ComponentStar> m_components;
};

}

#endif
