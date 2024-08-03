#pragma once 

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "Core/Parameter.h"

#include <string>
#include <vector>

// forward declare handlers
namespace SCT
{
int BC5(const SCL::ParameterInstance&, const std::string&);
int Verbose(const SCL::ParameterInstance&, const std::string&);
}

static const char* const kSCLTitle = "Star Catalog Tool";
// not const so it can be changed by flag handlers for verbosity etc.
extern bool kbSCLDisplayTitle;// = false;
static const SCL::Parameter kSCLParameters[] =
{
	{ "v", false, SCT::Verbose },
	{ "bc5", true, SCT::BC5 }
};

inline int Project_EntryPoint(const std::vector<SCL::ParameterInstance>&) { return 0; }

#endif
