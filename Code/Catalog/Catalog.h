#pragma once 

#ifndef CATALOG_H
#define CATALOG_H

#include "Star/Durchmusterung.h"
#include "Star/Star.h"

#include <functional>
#include <unordered_map>
#include <vector>

namespace SCT
{

class StarSystem;

class Catalog
{

public:

	static void AccumulateStar(const StarSystem& star);
	static void ForEachStar(std::function<void(StarSystem&)> eachFunction);

	static size_t Count() { return s_starEntries.size(); }

	static void Report();

private:

	static std::vector<StarSystem> s_starEntries;
	static std::unordered_map<int, size_t> s_hrMap;
	static std::unordered_map<int, size_t> s_hdMap;
	static std::unordered_map<int, size_t> s_hipMap;
	static std::unordered_map<int, size_t> s_saoMap;
	static std::unordered_map<int, size_t> s_fk5Map;
	static std::unordered_map<int, size_t> s_adsMap;
	static std::unordered_map<Durchmusterung, size_t> s_bdMap;
};

}

#endif
