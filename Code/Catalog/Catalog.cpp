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

template<typename LookupType>
void UpdateMap(
	std::unordered_map<LookupType, size_t>& map,
	LookupType (StarSystem::* lookup)() const,
	LookupType (ComponentStar::* componentLookup)() const,
	size_t index)
{
	const StarSystem& system = Catalog::GetStarSystem(index);
	LookupType baseLookup = (system.*lookup)();
	if(baseLookup != 0)
	{
		map[baseLookup] = index;
		for (int i = 0; i < system.GetComponentCount(); ++i)
		{
			const ComponentStar& star = system.GetComponent(i);
			const LookupType componentValue = (star.*componentLookup)();
			if (componentValue != 0)
			{
				map[componentValue] = index;
			}
		}
	}
}

template<>
void UpdateMap<Durchmusterung>(
	std::unordered_map<Durchmusterung, size_t>& map,
	Durchmusterung (StarSystem::* lookup)() const,
	Durchmusterung(ComponentStar::* componentLookup)() const,
	size_t index)
{
	const StarSystem& system = Catalog::GetStarSystem(index);
	Durchmusterung baseLookup = (system.*lookup)();
	if (baseLookup != Durchmusterung::Empty)
	{
		map[baseLookup.GetComponentless()] = index;
	}
}

void Catalog::RecreateLookups()
{
	s_hrMap.clear();
	s_hdMap.clear();
	s_hipMap.clear();
	s_saoMap.clear();
	s_fk5Map.clear();
	s_adsMap.clear();
	s_bdMap.clear();

	for (size_t i = 0; i < s_starEntries.size(); ++i)
	{
		UpdateMap(s_hrMap, &StarSystem::GetHR, &ComponentStar::GetHR, i);
		UpdateMap(s_hdMap, &StarSystem::GetHD, &ComponentStar::GetHD, i);
		UpdateMap(s_hipMap, &StarSystem::GetHip, &ComponentStar::GetHip, i);
		UpdateMap(s_saoMap, &StarSystem::GetSAO, &ComponentStar::GetSAO, i);
		UpdateMap(s_fk5Map, &StarSystem::GetFK5, &ComponentStar::GetFK5, i);
		UpdateMap(s_adsMap, &StarSystem::GetADS, &ComponentStar::GetADS, i);
		UpdateMap(s_bdMap, &StarSystem::GetDM, &ComponentStar::GetDM, i);
	}
}

}
