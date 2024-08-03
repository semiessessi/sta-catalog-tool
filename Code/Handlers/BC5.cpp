#include "Core/Parameter.h"

#include <cstdlib>

namespace SCT
{

int BC5(const SCL::ParameterInstance&, const std::string& value)
{
	// check the file exists. and check its the right file.
	FILE* pFile = fopen(value.c_str(), "rb");
	if (pFile == nullptr)
	{
		return -1;
	}
	fclose(pFile);

	return 0;
}

}
