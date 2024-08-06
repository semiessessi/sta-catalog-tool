#include "Star.h"

namespace SCT
{

Star::Star(StarCatalog catalog)
: m_RA(0)
, m_Dec(0)
, m_Hip(0)
, m_HR(0)
, m_HD(0)
, m_catalog(catalog)
{
}

Star::Star(
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

void Star::SetRA(const int hours, const int minutes, const double seconds)
{
	m_RA = RightAscension(hours, minutes, seconds);
}

void Star::SetDeclination(const int degrees, const int minutes, const double seconds)
{
	m_Dec = Declination(degrees, minutes, seconds);
}

std::string Star::GetNameString() const
{
	return m_crappyNameString;
}

}
