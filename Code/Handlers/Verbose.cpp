#include "Core/Parameter.h"
#include "Core/Project_Config.h"

#include <string>

bool kbSCLDisplayTitle = false;

namespace SCT
{

bool IsVerbose()
{
	return kbSCLDisplayTitle;
}

int Verbose(const SCL::ParameterInstance&, const std::string&)
{
	kbSCLDisplayTitle = true;
	return 0;
}

}
