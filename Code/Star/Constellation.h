#pragma once 

#ifndef CONSTELLATION_H
#define CONSTELLATION_H

#include <string>

namespace SCT
{

// this can fit in 7-bits
enum Constellation : unsigned int
{
	Unknown = 0,
	Andromeda,
	Antlia,
	Apus,
	Aquarius,
	Aquila,
	Ara,
	Aries,
	Auriga,
	Bootes,
	Caelum,
	Camelopardalis,
	Cancer,
	Canes_Venatici,
	Canis_Major,
	Canis_Minor,
	Capricornus,
	Carina,
	Cassiopeia,
	Centaurus,
	Cepheus,
	Cetus,
	Chamaeleon,
	Circinus,
	Columbia,
	Coma_Berenices,
	Corona_Australis,
	Corona_Borealis,
	Corvus,
	Crater,
	Crux,
	Cygnus,
	Delphinus,
	Dorado,
	Draco,
	Equuleus,
	Eridanus,
	Fornax,
	Gemini,
	Grus,
	Hercules,
	Horologium,
	Hydra,
	Hydrus,
	Indus,
	Lacerta,
	Leo,
	Leo_Minor,
	Lepus,
	Libra,
	Lupus,
	Lynx,
	Lyra,
	Mensa,
	Microscopium,
	Monoceros,
	Musca,
	Norma,
	Octans,
	Ophiuchus,
	Orion,
	Pavo,
	Pegasus,
	Perseus,
	Phoenix,
	Pictor,
	Pisces,
	Piscis_Austrinus,
	Puppis,
	Pyxis,
	Reticulum,
	Sagitta,
	Sagittarius,
	Scorpius,
	Sculptor,
	Scutum,
	Serpens,
	Sextans,
	Taurus,
	Telescopium,
	Triangulum,
	Triangulum_Australe,
	Tucana,
	Ursa_Major,
	Ursa_Minor,
	Vela,
	Virgo,
	Volans,
	Vulpecula
};

std::string GetConstellationName(const Constellation constellation);
std::string GetConstellationGenitive(const Constellation constellation);
Constellation GetConstellationFromAbbreviation(const std::string& abbreviation);

}

#endif
