#include "Catalog.h"

#include <cstdlib>

namespace SCT
{

bool IsVerbose();

void Catalog::Report()
{
	if (!IsVerbose())
	{
		return;
	}

	puts("Catalog content is now:");
	printf("Total star systems: %d\n", static_cast<int>(s_starEntries.size()));
	printf("HR stars: %d\n", static_cast<int>(s_hrMap.size()));
	printf("HD stars: %d\n", static_cast<int>(s_hdMap.size()));
	printf("Hip stars: %d\n", static_cast<int>(s_hipMap.size()));
	printf("SAO stars: %d\n", static_cast<int>(s_saoMap.size()));
	printf("FK5 stars: %d\n", static_cast<int>(s_fk5Map.size()));
	printf("ADS stars: %d\n", static_cast<int>(s_adsMap.size()));
}

}
