#include "Constellation.h"

namespace SCT
{

struct ConstellationData
{
	Constellation constellation;
	std::string abbreviation;
	std::string name;
	std::string genitive;
};

static const ConstellationData kConstellationData[89] =
{
	{ Unknown, "???", "Unknown", "" },
	{ Andromeda, "And", "Andromeda", "Andromedae" },
	{ Antlia, "Ant", "Antlia", "Antliae" },
	{ Apus, "Aps", "Apus", "Apodis" },
	{ Aquarius, "Aqr", "Aquarius", "Aquarii" },
	{ Aquila, "Aql", "Aquila", "Aquilae" },
	{ Ara, "Ara", "Ara", "Arae" },
	{ Aries, "Ari", "Aries", "Aries" },
	{ Auriga, "Aur", "Auriga", "Aurigae" },
	{ Bootes, "Boo", "Bootes", "Bootis" },
	{ Caelum, "Cae", "Caelum", "Caeli" },
	{ Camelopardalis, "Cam", "Camelopardalis", "Camelopardalis" },
	{ Cancer, "Cancer", "Cnc", "Cancri" },
	{ Canes_Venatici, "CVa", "Canes Venatici", "Canes Venaticorum" },
	{ Canis_Major, "CMa", "Canis Major", "Canis Majoris" },
	{ Canis_Minor, "CMi", "Canis Minor", "Canis Minoris" },
	{ Capricornus, "Cap", "Capricornus", "Capricorni" },
	{ Carina, "Car", "Carina", "Carinae" },
	{ Cassiopeia, "Cas", "Cassiopeia", "Cassiopeiae" },
	{ Centaurus, "Cen", "Centaurus", "Centauri" },
	{ Cepheus, "Cep", "Cepheus", "Cephei" },
	{ Cetus, "Cet", "Cetus", "Ceti" },
	{ Chamaeleon, "Cha", "Chamaeleon", "Chamaeleontis" },
	{ Circinus, "Cir", "Circinus", "Circini" },
	{ Columbia, "Col", "Columbia", "Columbi" },
	{ Coma_Berenices, "Com", "Coma Berenices", "Coma Berenices" },
	{ Corona_Australis, "CrA", "Corona Australis", "Corona Australi" },
	{ Corona_Borealis, "CrB", "Corona Borealis", "Corona Borealis" },
	{ Corvus, "Crv", "Corvus", "Corvi" },
	{ Crater, "Crt", "Crater", "Crateris" },
	{ Crux, "Cru", "Crux", "Crucis" },
	{ Cygnus, "Cyg", "Cygnus", "Cygni" },
	{ Delphinus, "Del", "Delphinus", "Delphini" },
	{ Dorado, "Dor", "Dorado", "Doradus" },
	{ Draco, "Dra", "Draco", "Draconis" },
	{ Equuleus, "Eql", "Equuleus", "Equuleui" },
	{ Eridanus, "Eri", "Eridanus", "Eridani" },
	{ Fornax, "For", "Fornax", "Fornacis" },
	{ Gemini, "Gem", "Gemini", "Geminorum" },
	{ Grus, "Grs", "Grus", "Grucis" },
	{ Hercules, "Her", "Hercules", "Herculis" },
	{ Horologium, "Hor", "Horologium", "Horologii" },
	{ Hydra, "Hya", "Hydra", "Hydrae" },
	{ Hydrus, "Hyi", "Hydrus", "Hydri" },
	{ Indus, "Ind", "Indus", "Indi" },
	{ Lacerta, "Lac", "Lacerta", "Lacertae" },
	{ Leo, "Leo", "Leo", "Leonis" },
	{ Leo_Minor, "LMi", "Leo Minor", "Leo Minoris" },
	{ Lepus, "Lep", "Lepus", "Lepidae" },
	{ Libra, "Lib", "Libra", "Librae" },
	{ Lupus, "Lup", "Lupus", "Lupi" },
	{ Lynx, "Lyn", "Lynx", "Lyncis" },
	{ Lyra, "Lyr", "Lyra", "Lyrae" },
	{ Mensa, "Men", "Mensa", "Mensae" },
	{ Microscopium, "Mic", "Microscopium", "Microscopii" },
	{ Monoceros, "Mon", "Monoceros", "Monocerotis" },
	{ Musca, "Mus", "Musca", "Muscae" },
	{ Norma, "Nor", "Norma", "Normae" },
	{ Octans, "Oct", "Octans", "Octantis" },
	{ Ophiuchus, "Oph", "Ophiuchus", "Ophiuchii" },
	{ Orion, "Ori", "Orion", "Orionis" },
	{ Pavo, "Pav", "Pavo", "Pavonis" },
	{ Pegasus, "Peg", "Pegasus", "Pegasi" },
	{ Perseus, "Per", "Perseus", "Persei" },
	{ Phoenix, "Pho", "Phoenix", "Phoenicis" },
	{ Pictor, "Pic", "Pictor", "Pictoris" },
	{ Pisces, "Psc", "Pisces", "Piscis" },
	{ Piscis_Austrinus, "PsA", "Piscis Austrinus", "Piscis Austrini" },
	{ Puppis, "Pup", "Puppis", "Puppis" },
	{ Pyxis, "Pyx", "Pyxis", "Pyxidis" },
	{ Reticulum, "Ret", "Reticulum", "Reticuli" },
	{ Sagitta, "Sgt", "Sagitta", "Sagittae" },
	{ Sagittarius, "Sag", "Sagittarius", "Sagittarii" },
	{ Scorpius, "Sco", "Scorpius", "Scorpii" },
	{ Sculptor, "Scl", "Sculptor", "Sculptoris" },
	{ Scutum, "Sct", "Scutum", "Scuti" },
	{ Serpens, "Ser", "Serpens", "Serpentis" },
	{ Sextans, "Sex", "Sextans", "Sextantis" },
	{ Taurus, "Tau", "Taurus", "Tauri" },
	{ Telescopium, "Tel", "Telescopium", "Telescopii" },
	{ Triangulum, "Tri", "Triangulum", "Trianguli" },
	{ Triangulum_Australe, "TrA", "Triangulum Australe", "Triangulum Australis" },
	{ Tucana, "Tucana", "Tucana", "Tucanae" },
	{ Ursa_Major, "UMa", "Ursa Major", "Ursa Majoris" },
	{ Ursa_Minor, "UMi", "Ursa Minor", "Ursa Minoris" },
	{ Vela, "Vel", "Vela", "Velorum" },
	{ Virgo, "Vir", "Virgo", "Virginis" },
	{ Volans, "Vol", "Volans", "Volantis" },
	{ Vulpecula, "Vul", "Vulpecula", "Vulpeculae" },
};

std::string GetConstellationName(const Constellation constellation)
{
	return kConstellationData[constellation].name;
}

Constellation GetConstellationFromAbbreviation(const std::string& abbreviation)
{
	for (int i = 0; i < 89; ++i)
	{
		if (kConstellationData[i].abbreviation == abbreviation)
		{
			return kConstellationData[i].constellation;
		}
	}

	return Unknown;
}

}
