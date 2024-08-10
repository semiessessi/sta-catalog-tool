#pragma once 

#ifndef STAR_CATALOG_H
#define STAR_CATALOG_H

namespace SCT
{

enum StarCatalog : unsigned char
{
	HENRY_DRAPER_CATALOG = 0,														// https://cdsarc.cds.unistra.fr/viz-bin/cat/V/103
	HENRY_DRAPER_EXTENDED_CHARTS = 1,												// https://cdsarc.cds.unistra.fr/ftp/cats/III/182/
	SMITHSONIAN_ASTROPHYSICAL_OBSERVATORY_STAR_CATALOG = 2,							// https://cdsarc.cds.unistra.fr/ftp/cats/I/131A/
	YALE_BRIGHT_STAR_CATALOG_5_V_50 = 3,											// http://tdc-www.harvard.edu/catalogs/bsc5.html
	HIPPARCOS = 4,																	// https://cdsarc.cds.unistra.fr/viz-bin/cat/I/239
};

}

#endif