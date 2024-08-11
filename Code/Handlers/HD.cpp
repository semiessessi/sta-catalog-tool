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
	static std::string dm(12, ' ');
	dm = line.substr(6, 12);
	if (dm[9] != ' ')
	{
		star.SetDM(Durchmusterung::FromStringWithComponent(dm));
	}

	// 19-20
	static std::string len2String(2, ' ');
	len2String = line.substr(18, 2);
	const int raHours = std::stoi(len2String);
	// 21-23
	static std::string len3String(3, ' ');
	len3String = line.substr(20, 3);
	const int raDeciminutes = std::stoi(len3String);
	const int raMinutes = raDeciminutes / 10;
	const double raSeconds = .1 * (raDeciminutes % 10);
	star.SetRA(raHours, raMinutes, raSeconds);

	// 24
	const bool decNegative = line[23] == '-';
	// 25-26
	len2String = line.substr(24, 2);
	const int decDegrees = std::stoi(len2String);
	// 27-28
	len2String = line.substr(26, 2);
	const int decMinutes = std::stoi(len2String);
	star.SetDeclination(decNegative ? -decDegrees : decDegrees, decMinutes, 0);

	// 30-34 Visual magnitude
	static std::string len5String(3, ' ');
	if (line[31] != ' ')
	{
		len5String = line.substr(29, 5);
		const float vmag = std::stof(len5String);
		star.SetVisualMagnitude(vmag);
	}
	else if (line[38] != ' ') // 30-34 Visual magnitude
	{
		len5String = line.substr(36, 5);
		const float vmag = std::stof(len5String);
		star.SetVisualMagnitude(vmag);
	}

	// 43-45
	// spectral type
	if ((line.size() > 43) && (line[42] != ' '))
	{
		len3String = line.substr(42, 3);
		star.SetSpectralClass(SpectralClass::FromString(len3String));
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
