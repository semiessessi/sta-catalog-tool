#include "Core/Parameter.h"

#include <cstdlib>

namespace SCT
{

void ProcessBC5Line(const std::string& path)
{

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
