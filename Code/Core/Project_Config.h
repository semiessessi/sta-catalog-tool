#pragma once 

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "Core/Parameter.h"

#include <string>

// forward declare handlers
namespace SCT
{
int Verbose(const SCL::ParameterInstance&, const std::string&);
}

static const char* const kSCLTitle = "Star Catalog Tool";
// not const so it can be changed by flag handlers for verbosity etc.
extern bool kbSCLDisplayTitle;// = false;
static const SCL::Parameter kSCLParameters[] =
{
	{ "v", false, SCT::Verbose },
};

#endif
