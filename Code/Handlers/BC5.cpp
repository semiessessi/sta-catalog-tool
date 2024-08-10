#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Star/Star.h"

#include <cstdlib>


// data source: http://tdc-www.harvard.edu/catalogs/bsc5.html

namespace SCT
{

bool IsTest();
bool IsVerbose();

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

	const auto bayer = ParseBayerFromName(name.substr(3, 4));
	star.SetBayer(bayer.first, bayer.second);

	star.SetConstellation(GetConstellationFromAbbreviation(name.substr(7, 3)));

	// Durchmusterung 15-25
	const std::string dm = line.substr(14, 11);
	star.SetDM(Durchmusterung::FromHipString(dm));

	// HD 26-31
	if (line[30] != ' ')
	{
		const int hd = std::stoi(line.substr(25, 6));
		star.SetHD(hd);
	}

	// SAO 32-37
	if (line[36] != ' ')
	{
		const int sao = std::stoi(line.substr(31, 6));
		star.SetSAO(sao);
	}

	// FK5 38-41
	if (line[40] != ' ')
	{
		const int fk5 = std::stoi(line.substr(37, 4));
		star.SetFK5(fk5);
	}

	// ADS 45-49
	if (line[48] != ' ')
	{
		const int ads = std::stoi(line.substr(44, 5));
		star.SetADS(ads);
	}

	const char componentLetter = line[49];
	if (componentLetter != ' ')
	{
		star.SetComponentCode(componentLetter);
	}

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
	if ((line[106] != ' ') || (line[105] != ' '))
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
	if (IsVerbose())
	{
		puts("Reading Yale Bright Star Catalog...");
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
			ProcessBC5Line(data.substr(lastStart, i - lastStart));
			lastStart = i + 1;
		}
	}

	if (IsVerbose())
	{
		puts("Done.");
	}

	SCT::Catalog::Report();

	if (IsTest())
	{
		// check that every HR star has a magnitude
		// count the alphas for a test
		{
			int noMagnitudeCount = 0;
			int alphaCount = 0;
			Catalog::ForEachStar([&](SCT::StarSystem& starSystem)
				{
					if (starSystem.GetHR() != 0)
					{
						if (starSystem.GetVisualMagnitude() > 50.0f)
						{
							++noMagnitudeCount;
						}

						if (starSystem.GetBayer() == Alpha)
						{
							if (starSystem.GetBayerIndex() < 2)
							{
								++alphaCount;
							}
						}
					}
				});

			if (noMagnitudeCount > 0)
			{
				printf("ERROR: %d HR stars are missing visual magnitudes!\n", noMagnitudeCount);
				return 1;
			}
			else
			{
				puts("All stars from HR catalog seem to have valid visual magnitudes");
			}

			if (alphaCount != 84)
			{
				printf("ERROR: Test failed! %d HR stars are alpha or alpha-1 - expected 84!\n", alphaCount);
				return 1;
			}
			else
			{
				puts("All alpha stars accounted for");
			}
		}
	}

	return 0;
}

}
