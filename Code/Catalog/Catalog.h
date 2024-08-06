#pragma once 

#ifndef CATALOG_H
#define CATALOG_H

#include "Star/Star.h"

#include <functional>
#include <vector>

namespace SCT
{

class Star;

class Catalog
{

public:

	static void AccumulateStar(const Star& star);
	static void ForEachStar(std::function<void(Star&)> eachFunction);

	static size_t Count() { return s_starEntries.size(); }

private:

	static std::vector<Star> s_starEntries;
};

}

#endif
