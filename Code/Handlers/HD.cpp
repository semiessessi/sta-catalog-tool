#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Star/Star.h"

#include <cstdlib>


// data source: https://cdsarc.cds.unistra.fr/viz-bin/cat/III/135A

namespace SCT
{

bool IsVerbose();

void ProcessHDLine(const std::string& line)
{
	// first few characters are the index
	const int index = std::stoi(line.substr(0, 6));
	StarSystem star(StarCatalog::HENRY_DRAPER_CATALOG, index);

	// Durchmusterung 7-18
	const std::string dm = line.substr(6, 12);
	if (dm[9] != ' ')
	{
		star.SetDM(Durchmusterung::FromHDString(dm));
	}

	// 19-20
	const int raHours = std::stoi(line.substr(18, 2));
	// 21-23
	const int raDeciminutes = std::stoi(line.substr(20, 3));
	const int raMinutes = raDeciminutes / 10;
	const double raSeconds = .1 * (raDeciminutes % 10);
	star.SetRA(raHours, raMinutes, raSeconds);

	// 24
	const bool decNegative = line[23] == '-';
	// 25-26
	const int decDegrees = std::stoi(line.substr(24, 2));
	// 27-28
	const int decMinutes = std::stoi(line.substr(26, 2));
	star.SetDeclination(decNegative ? -decDegrees : decDegrees, decMinutes, 0);

	// 30-34 Visual magnitude
	if (line[31] != ' ')
	{
		const float vmag = std::stof(line.substr(29, 5));
		star.SetVisualMagnitude(vmag);
	}
	else if (line[38] != ' ') // 30-34 Visual magnitude
	{
		const float vmag = std::stof(line.substr(36, 5));
		star.SetVisualMagnitude(vmag);
	}

	// 43-45
	// spectral type
	if ((line.size() > 43) && (line[42] != ' '))
	{
		star.SetSpectralClass(SpectralClass::FromString(line.substr(42,3)));
	}

	Catalog::AccumulateStar(star);
}

int HD(const SCL::ParameterInstance&, const std::string& value)
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

int ReadHDData(const std::string& path)
{
	if (IsVerbose())
	{
		puts("Reading Henry Draper Star Catalog...");
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
			ProcessHDLine(data.substr(lastStart, i - lastStart));
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
