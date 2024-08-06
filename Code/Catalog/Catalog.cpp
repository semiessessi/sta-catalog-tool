#include "Catalog.h"

namespace SCT
{

std::vector<Star> Catalog::s_starEntries;

void Catalog::AccumulateStar(const Star& star)
{
	s_starEntries.push_back(star);
}

void Catalog::ForEachStar(std::function<void(Star&)> eachFunction)
{
	for (size_t i = 0; i < s_starEntries.size(); ++i)
	{
		eachFunction(s_starEntries[i]);
	}
}

}
