#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Star/Star.h"

#include <cstdlib>

namespace SCT
{

void ProcessBC5Line(const std::string& line)
{
	// is the star even real?
	if (line[83] == ' ')
	{
		return;
	}

	// first few characters are the index
	const int index = std::stoi(line.substr(0, 4));
	StarSystem star(StarCatalog::YALE_BRIGHT_STAR_CATALOG_5_V_50, index);

	// name 5-14
	const std::string name = line.substr(4, 10);
	star.SetNameString(name);
	const int flamsteed = (name[2] != ' ') ? std::stoi(name.substr(0, 3)) : 0;

	star.SetFlamsteed(flamsteed);

	// Durchmusterung 15-25
	const std::string dm = line.substr(14, 11);

	// HD 26-31
	const int hd = std::stoi(line.substr(25, 6));
	star.SetHD(hd);

	// 76-77
	const int raHours = std::stoi(line.substr(75, 2));
	// 78-79
	const int raMinutes = std::stoi(line.substr(77, 2));
	// 80-83
	const double raSeconds = std::stod(line.substr(79, 4));
	star.SetRA(raHours, raMinutes, raSeconds);

	// 84
	const bool decNegative = line[83] == '-';
	// 85-86
	const int decDegrees = std::stoi(line.substr(84, 2));
	// 87-88
	const int decMinutes = std::stoi(line.substr(86, 2));
	// 89-90
	const int decSeconds = std::stoi(line.substr(88, 2));

	star.SetDeclination(decNegative ? -decDegrees : decDegrees, decMinutes, decSeconds);

	// 103-107 Visual magnitude
	if (line[106] != ' ')
	{
		const float vmag = std::stof(line.substr(102, 5));
		star.SetVisualMagnitude(vmag);
	}

	// 110-114 B-V color in the UBV system
	if (line[113] != ' ')
	{
		const float bmv = std::stof(line.substr(109, 5));
		star.SetBMinusV(bmv);
	}

	Catalog::AccumulateStar(star);
}

int BC5(const SCL::ParameterInstance&, const std::string& value)
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

int ReadBC5Data(const std::string& path)
{
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
			ProcessBC5Line(data.substr(lastStart, i - lastStart));
			lastStart = i + 1;
		}
	}

	return 0;
}

}
