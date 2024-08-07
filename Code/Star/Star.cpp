#include "Star.h"

namespace SCT
{

StarSystem::StarSystem(StarCatalog catalog)
: m_RA(0)
, m_Dec(0)
, m_Hip(0)
, m_HR(0)
, m_HD(0)
, m_catalog(catalog)
{
}

StarSystem::StarSystem(
	const StarCatalog catalog,
	const uint64_t sourceCatalogIndex)
: m_RA(0)
, m_Dec(0)
, m_Hip(0)
, m_HR((catalog == YALE_BRIGHT_STAR_CATALOG_5_V_50)
	? static_cast<unsigned short>(sourceCatalogIndex)
	: 0)
, m_HD(0)
, m_catalog(catalog)
{
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

}
