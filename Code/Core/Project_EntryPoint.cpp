#include "Core/Parameter.h"

#include <vector>

namespace SCT
{
int ReadBC5Data(const std::string& path);
void FindPotentialDecans();
}

int Project_EntryPoint(const std::vector<SCL::ParameterInstance>& parameters)
{
	int worstWarning = 0;
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		if (parameters[i].parameter.name == "bc5")
		{
			int returnValue = SCT::ReadBC5Data(parameters[i].value);
			if (returnValue < 0)
			{
				return returnValue;
			}

			if (returnValue > worstWarning)
			{
				worstWarning = returnValue;
			}
		}
		else if (parameters[i].parameter.name == "decans")
		{
			SCT::FindPotentialDecans();
		}
	}

	return worstWarning;
}