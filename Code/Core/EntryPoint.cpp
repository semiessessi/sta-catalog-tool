// SE - NOTE: this file is just plumbing for the library
#include "Core/Project_Config.h"

namespace SCL
{
int EntryPoint(const int argumentCount, const char* const* const arguments/*, const SCL::Parameter* const parameters*/);
}

int main(const int argumentCount, const char* const* const arguments)
{
	return SCL::EntryPoint(argumentCount, arguments/*, kSCLParameters*/);
}
