#include "Star.h"

#include <format>

namespace SCT
{

std::pair<BayerGreekLetter, unsigned char> ParseBayerFromName(const std::string& name);

ComponentStar ComponentStar::FromCatalog(const StarCatalog source)
{
	ComponentStar returnValue;
	returnValue.m_catalog = source;
	return returnValue;
}

void ComponentStar::Merge(const StarSystem& system)
{
	if (GetHR() == 0)
	{
		SetHR(system.GetHR());
	}

	if (GetHD() == 0)
	{
		SetHD(system.GetHD());
	}

	if (GetHip() == 0)
	{
		SetHip(system.GetHip());
	}

	if (GetSAO() == 0)
	{
		SetSAO(system.GetSAO());
	}

	if (GetFK5() == 0)
	{
		SetSAO(system.GetFK5());
	}

	if (GetADS() == 0)
	{
		SetADS(system.GetADS());
	}

	if (GetFlamsteed() == 0)
	{
		m_flamsteed = system.GetFlamsteed();
	}

	if ((GetBayer() == None)
		&& (system.GetBayer() != None))
	{
		m_bayerLetter = system.GetBayer();
	}

	if ((GetBayerIndex() == 0)
		&& (system.GetBayerIndex() != 0))
	{
		m_bayerIndex = system.GetBayerIndex();
	}

	if (GetVisualMagnitude() >= 50.0)
	{
		m_visualMagnitude = system.GetVisualMagnitude();
	}

	if (GetBMinusV() >= 50.0)
	{
		m_BminusVMagnitude = system.GetBMinusV();
	}

	if (GetSpectralClass() == SpectralClass::Empty)
	{
		m_spectralClass = system.GetSpectralClass();
	}

	if (GetCatalog() < system.GetCatalog())
	{
		if (system.GetVisualMagnitude() < 50.0)
		{
			m_visualMagnitude = system.GetVisualMagnitude();
		}

		if (system.GetSpectralClass() != SpectralClass::Empty)
		{
			m_spectralClass = system.GetSpectralClass();
		}

		m_catalog = system.GetCatalog();
	}
}

void ComponentStar::SetFromSystemPrimary(const StarSystem& system)
{
	m_HR = system.GetHR();
	m_HD = system.GetHD();
	m_ADS = system.GetADS();
	m_Hip = system.GetHip();
	m_FK5 = system.GetFK5();
	m_SAO = system.GetSAO();
	m_flamsteed = system.GetFlamsteed();
	m_bayerLetter = system.GetBayer();
	m_bayerIndex = system.GetBayerIndex();
	m_visualMagnitude = system.GetVisualMagnitude();
	m_BminusVMagnitude = system.GetBMinusV();
}

StarSystem::StarSystem(StarCatalog catalog)
: m_RA(0)
, m_Dec(0)
{
	m_components.push_back(ComponentStar::FromCatalog(catalog));
}

StarSystem::StarSystem(
	const StarCatalog catalog,
	const uint64_t sourceCatalogIndex)
: m_RA(0)
, m_Dec(0)
{
	m_components.push_back(ComponentStar::FromCatalog(catalog));
	if(catalog == YALE_BRIGHT_STAR_CATALOG_5_V_50)
	{
		m_components[0].SetHR(static_cast<int>(sourceCatalogIndex));
	}
	else if (catalog == HIPPARCOS)
	{
		m_components[0].SetHip(static_cast<int>(sourceCatalogIndex));
	}
	else if (catalog == HENRY_DRAPER_EXTENDED_CHARTS)
	{
		m_components[0].SetHD(static_cast<int>(sourceCatalogIndex));
	}
	else if (catalog == HENRY_DRAPER_CATALOG)
	{
		m_components[0].SetHD(static_cast<int>(sourceCatalogIndex));
	}
	else if (catalog == SMITHSONIAN_ASTROPHYSICAL_OBSERVATORY_STAR_CATALOG)
	{
		m_components[0].SetSAO(static_cast<int>(sourceCatalogIndex));
	}
}

void StarSystem::SetRA(const int hours, const int minutes, const double seconds)
{
	m_RA = RightAscension(hours, minutes, seconds);
}

void StarSystem::SetDeclination(const int degrees, const int minutes, const double seconds)
{
	m_Dec = Declination(degrees, minutes, seconds);
}

std::string StarSystem::GetNameString() const
{
	return m_crappyNameString;
}

void StarSystem::AccumulateNewData(const StarSystem& dataSource)
{
	// SE - TODO: fix this up.
	// if any of the numbers differ then add a new component
	// otherwise match an existing one
	int match = -1;
	if (dataSource.GetComponentCode() != 0)
	{
		for (int i = 0; i < m_components.size(); ++i)
		{
			if (dataSource.GetComponentCode()
				== m_components[i].GetComponentCode())
			{
				match = i;
				break;
			}
		}
	}
	else if(dataSource.GetHD() != 0)
	{
		// TODO:...
		for (int i = 0; i < m_components.size(); ++i)
		{
			if (dataSource.GetHD()
				== m_components[i].GetHD())
			{
				match = i;
				break;
			}
		}
	}
	else if (dataSource.GetSAO() != 0)
	{
		// TODO:...
		for (int i = 0; i < m_components.size(); ++i)
		{
			if (dataSource.GetSAO()
				== m_components[i].GetSAO())
			{
				match = i;
				break;
			}
		}
	}

	// no match
	if (match == -1)
	{
		ComponentStar star = ComponentStar::FromCatalog(dataSource.GetCatalog());
		star.SetFromSystemPrimary(dataSource);
		m_components.push_back(star);
	}
	else
	{
		m_components[match].Merge(dataSource);
	}
}

}
