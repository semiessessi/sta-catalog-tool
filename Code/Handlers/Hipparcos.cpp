#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Star/Star.h"

#include <cstdlib>

// data source: https://cdsarc.cds.unistra.fr/ftp/cats/I/239/

namespace SCT
{

bool IsVerbose();

void ProcessHipLine(const std::string& line)
{
	// is the star even real?
	if (line[0] != 'H')
	{
		return;
	}

	// first few characters are the index
	const int index = std::stoi(line.substr(8, 6));
	StarSystem star(StarCatalog::HIPPARCOS, index);

	// name 5-14
	//const std::string name = line.substr(4, 10);
	//star.SetNameString(name);
	//const int flamsteed = (name[2] != ' ') ? std::stoi(name.substr(0, 3)) : 0;
	//
	//star.SetFlamsteed(flamsteed);

	// 398-407 Durchmusterung
	std::string dm = line.substr(397, 10);
	if (dm[0] == 'B')
	{
		dm.insert(dm.begin() + 1, 'D');
		star.SetDM(Durchmusterung::FromComponentFreeString(dm));
	}
	else
	{
		// 409-418 CD
		dm = line.substr(408, 10);
		if (dm[0] == 'C')
		{
			dm.insert(dm.begin() + 1, 'D');
			star.SetDM(Durchmusterung::FromComponentFreeString(dm));
		}
		else
		{
			// 420-429 CP
			dm = line.substr(419, 10);
			if (dm[0] == 'P')
			{
				dm.insert(dm.begin(), 'C');
				star.SetDM(Durchmusterung::FromComponentFreeString(dm));
			}
		}
	}

	// HD 391-396
	if (line[395] != ' ')
	{
		const int hd = std::stoi(line.substr(390, 6));
		star.SetHD(hd);
	}

	// TODO: adjust with proper motion.
	if (line[51] != ' ')
	{
		const double ra = std::stod(line.substr(51, 11));
		star.SetRA(RightAscension::FromDegrees(ra));
	}
	else
	{
		return;
	}

	if (line[64] != ' ')
	{
		const double dec = std::stod(line.substr(64, 11));
		star.SetDeclination(Declination::FromDegrees(dec));
	}
	else
	{
		return;
	}

	if (line[435] != ' ')
	{
		star.SetSpectralClass(SpectralClass::FromString(line.substr(435, 5)));
	}

	// 42-46 Visual magnitude
	if (line[43] != ' ')
	{
		const float vmag = std::stof(line.substr(41, 5));
		star.SetVisualMagnitude(vmag);
	}
	
	//// 110-114 B-V color in the UBV system
	//if (line[113] != ' ')
	//{
	//	const float bmv = std::stof(line.substr(109, 5));
	//	star.SetBMinusV(bmv);
	//}

	Catalog::AccumulateStar(star);
}

int Hip(const SCL::ParameterInstance&, const std::string& value)
{
	// check the file exists. and check its the right file.
	FILE* pFile = fopen(value.c_str(), "rb");
	if (pFile == nullptr)
	{
		printf("ERROR: Could not open file %s\n", value.c_str());
		return -1;
	}

	fclose(pFile);

	return 0;
}

int ReadHipData(const std::string& path)
{
	if (IsVerbose())
	{
		puts("Reading HIPPARCOS Star Catalog...");
	}

	FILE* pFile = fopen(path.c_str(), "rb");
	if (pFile == nullptr)
	{
		// this should never happen
		printf("FATAL ERROR: Could not open file %s, despite checking it exists first!\n", path.c_str());
		return -1;
	}

	fseek(pFile, 0, SEEK_END);
	int dataSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	std::string data;
	data.resize(dataSize);
	fread(&(data[0]), 1, dataSize, pFile);
	fclose(pFile);

	// process the data
	int lastStart = 0;
	for (int i = 0; i < dataSize; ++i)
	{
		if (data[i] == '\n')
		{
			// we have a line.
			ProcessHipLine(data.substr(lastStart, i - lastStart));
			lastStart = i + 1;
		}
	}

	if (IsVerbose())
	{
		puts("Done.");
	}

	SCT::Catalog::Report();

	return 0;
}

}
