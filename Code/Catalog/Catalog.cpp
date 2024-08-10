#include "Catalog.h"

namespace SCT
{

std::vector<StarSystem> Catalog::s_starEntries;

std::unordered_map<int, size_t> Catalog::s_hrMap;
std::unordered_map<int, size_t> Catalog::s_hdMap;
std::unordered_map<int, size_t> Catalog::s_hipMap;
std::unordered_map<int, size_t> Catalog::s_saoMap;
std::unordered_map<int, size_t> Catalog::s_fk5Map;
std::unordered_map<int, size_t> Catalog::s_adsMap;

std::unordered_map<Durchmusterung, size_t> Catalog::s_bdMap;

void Catalog::AccumulateStar(const StarSystem& star)
{
	bool addNew = true;
	size_t starIndex = s_starEntries.size();

	if (star.GetHR() != 0)
	{
		if (s_hrMap.find(star.GetHR()) != s_hrMap.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_hrMap[star.GetHR()];
		}
	}

	if (star.GetHD() != 0)
	{
		if (s_hdMap.find(star.GetHD()) != s_hdMap.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_hdMap[star.GetHD()];
		}
	}

	if (star.GetHip() != 0)
	{
		if (s_hipMap.find(star.GetHip()) != s_hipMap.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_hipMap[star.GetHip()];
		}
	}

	if (star.GetFK5() != 0)
	{
		if (s_fk5Map.find(star.GetFK5()) != s_fk5Map.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_fk5Map[star.GetFK5()];
		}
	}

	if (star.GetSAO() != 0)
	{
		if (s_saoMap.find(star.GetSAO()) != s_saoMap.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_saoMap[star.GetSAO()];
		}
	}

	if (star.GetADS() != 0)
	{
		if (s_adsMap.find(star.GetADS()) != s_adsMap.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_adsMap[star.GetADS()];
		}
	}

	if (star.GetDMComponentless() != Durchmusterung::Empty)
	{
		if (s_bdMap.find(star.GetDMComponentless()) != s_bdMap.end())
		{
			// it already exists in this map...
			addNew = false;
			starIndex = s_bdMap[star.GetDMComponentless()];
		}
	}

	if (addNew)
	{
		s_starEntries.push_back(star);
	}
	else
	{
		s_starEntries[starIndex].AccumulateNewData(star);
	}

	if (star.GetADS() != 0)
	{
		s_adsMap[star.GetADS()] = starIndex;
	}

	if (star.GetSAO() != 0)
	{
		s_saoMap[star.GetSAO()] = starIndex;
	}

	if (star.GetHD() != 0)
	{
		s_hdMap[star.GetHD()] = starIndex;
	}

	if (star.GetHR() != 0)
	{
		s_hrMap[star.GetHR()] = starIndex;
	}

	if (star.GetHip() != 0)
	{
		s_hipMap[star.GetHip()] = starIndex;
	}

	if (star.GetFK5() != 0)
	{
		s_fk5Map[star.GetFK5()] = starIndex;
	}

	if (star.GetDMComponentless() != Durchmusterung::Empty)
	{
		s_bdMap[star.GetDMComponentless()] = starIndex;
	}
}

void Catalog::ForEachStar(std::function<void(StarSystem&)> eachFunction)
{
	for (size_t i = 0; i < s_starEntries.size(); ++i)
	{
		eachFunction(s_starEntries[i]);
	}
}

}
