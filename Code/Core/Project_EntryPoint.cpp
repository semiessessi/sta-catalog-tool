#include "Core/Parameter.h"

#include <vector>

namespace SCT
{
int ReadBC5Data(const std::string& path);
int ReadHipData(const std::string& path);
int ReadHDData(const std::string& path);
int ReadHDECData(const std::string& path);
int ReadSAOData(const std::string& path);
void FindPotentialDecans();
int DoTests();
}

int Project_EntryPoint(const std::vector<SCL::ParameterInstance>& parameters)
{
	int worstWarning = 0;
	bool doTests = false;
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
		else if (parameters[i].parameter.name == "hip")
		{
			int returnValue = SCT::ReadHipData(parameters[i].value);
			if (returnValue < 0)
			{
				return returnValue;
			}

			if (returnValue > worstWarning)
			{
				worstWarning = returnValue;
			}
		}
		else if (parameters[i].parameter.name == "hd")
		{
			int returnValue = SCT::ReadHDData(parameters[i].value);
			if (returnValue < 0)
			{
				return returnValue;
			}

			if (returnValue > worstWarning)
			{
				worstWarning = returnValue;
			}
		}
		else if (parameters[i].parameter.name == "hdec")
		{
			int returnValue = SCT::ReadHDECData(parameters[i].value);
			if (returnValue < 0)
			{
				return returnValue;
			}

			if (returnValue > worstWarning)
			{
				worstWarning = returnValue;
			}
		}
		else if (parameters[i].parameter.name == "sao")
		{
			int returnValue = SCT::ReadSAOData(parameters[i].value);
			if (returnValue < 0)
			{
				return returnValue;
			}

			if (returnValue > worstWarning)
			{
				worstWarning = returnValue;
			}
		}
		else if ((parameters[i].parameter.name == "test")
			|| (parameters[i].parameter.name == "t"))
		{
			doTests = true;
		}
		else if (parameters[i].parameter.name == "decans")
		{
			SCT::FindPotentialDecans();
		}
	}


	if (doTests)
	{
		int returnValue = SCT::DoTests();
		if (returnValue < 0)
		{
			return returnValue;
		}

		if (returnValue > worstWarning)
		{
			worstWarning = returnValue;
		}
	}

	return worstWarning;
}