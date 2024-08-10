#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Star/Star.h"

#include <cstdlib>


// data source: https://cdsarc.cds.unistra.fr/ftp/cats/III/182/

namespace SCT
{

bool IsVerbose();

void ProcessHDECLine(const std::string& line)
{
	// is the star even real?
	//if (line[0] != 'H')
	//{
	//	return;
	//}

	// first few characters are the index
	const int index = std::stoi(line.substr(0, 6));
	StarSystem star(StarCatalog::HENRY_DRAPER_EXTENDED_CHARTS, index);

	if (line[6] != ' ')
	{
		star.SetComponentCode(line[6]);
	}

	// 11-12 spectral type
	star.SetSpectralClass(SpectralClass::FromString(line.substr(10, 2)));

	// 20-21
	const int raHours = std::stoi(line.substr(19, 2));
	// 22-23
	const int raMinutes = std::stoi(line.substr(21, 2));
	// 27-31
	const double raSeconds = std::stod(line.substr(26, 5));
	star.SetRA(raHours, raMinutes, raSeconds);

	// 33
	const bool decNegative = line[32] == '-';
	// 34-35
	const int decDegrees = std::stoi(line.substr(33, 2));
	// 36-37
	const int decMinutes = std::stoi(line.substr(35, 2));
	// 38-41
	const int decSeconds = std::stoi(line.substr(37, 4));

	star.SetDeclination(decNegative ? -decDegrees : decDegrees, decMinutes, decSeconds);

	// 14-17 Visual magnitude
	if (line[14] != ' ')
	{
		const float vmag = std::stof(line.substr(13, 4));
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

int HDEC(const SCL::ParameterInstance&, const std::string& value)
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

int ReadHDECData(const std::string& path)
{
	if (IsVerbose())
	{
		puts("Reading Henry Draper Extended Star Catalog...");
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
			ProcessHDECLine(data.substr(lastStart, i - lastStart));
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
