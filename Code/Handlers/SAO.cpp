#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Star/Star.h"

#include <cstdlib>


// data source: https://cdsarc.cds.unistra.fr/ftp/cats/I/131A/

namespace SCT
{

bool IsVerbose();

void ProcessSAOLine(const std::string& line)
{
	// is the star even real?
	if (line[6] == 'D')
	{
		return;
	}

	// first few characters are the index
	const int index = std::stoi(line.substr(0, 6));
	StarSystem star(StarCatalog::SMITHSONIAN_ASTROPHYSICAL_OBSERVATORY_STAR_CATALOG, index);

	// HD 118-123
	if (line[122] != ' ')
	{
		const int hd = std::stoi(line.substr(117, 6));
		star.SetHD(hd);
	}

	// 85-87 spectral type
	star.SetSpectralClass(SpectralClass::FromString(line.substr(84, 3)));

	// 151-152
	const int raHours = std::stoi(line.substr(150, 2));
	// 153-154
	const int raMinutes = std::stoi(line.substr(152, 2));
	// 155-160
	const double raSeconds = std::stod(line.substr(154, 6));
	star.SetRA(raHours, raMinutes, raSeconds);

	// 168
	const bool decNegative = line[167] == '-';
	// 169-170
	const int decDegrees = std::stoi(line.substr(168, 2));
	// 171-172
	const int decMinutes = std::stoi(line.substr(170, 2));
	// 173-177
	const int decSeconds = std::stoi(line.substr(172, 5));

	star.SetDeclination(decNegative ? -decDegrees : decDegrees, decMinutes, decSeconds);

	// 81-84 Visual magnitude
	if (line[82] != ' ')
	{
		const float vmag = std::stof(line.substr(80, 5));
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

int SAO(const SCL::ParameterInstance&, const std::string& value)
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

int ReadSAOData(const std::string& path)
{
	if (IsVerbose())
	{
		puts("Reading Smithsonian Astrophysical Observatory Catalog...");
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
			ProcessSAOLine(data.substr(lastStart, i - lastStart));
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
