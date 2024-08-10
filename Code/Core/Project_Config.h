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
int Hip(const SCL::ParameterInstance&, const std::string&);
int HD(const SCL::ParameterInstance&, const std::string&);
int HDEC(const SCL::ParameterInstance&, const std::string&);
int SAO(const SCL::ParameterInstance&, const std::string&);
int Verbose(const SCL::ParameterInstance&, const std::string&);
int Test(const SCL::ParameterInstance&, const std::string&);
}

static const char* const kSCLTitle = "Star Catalog Tool";
// not const so it can be changed by flag handlers for verbosity etc.
extern bool kbSCLDisplayTitle;// = false;
static const SCL::Parameter kSCLParameters[] =
{
	{ "v", false, SCT::Verbose },
	{ "verbose", false, SCT::Verbose },
	{ "t", false, SCT::Test },
	{ "test", false, SCT::Test },
	{ "bc5", true, SCT::BC5 },
	{ "decans" },
	{ "hip", true, SCT::Hip },
	{ "hd", true, SCT::HD },
	{ "hdec", true, SCT::HDEC },
	{ "sao", true, SCT::SAO },
};

int Project_EntryPoint(const std::vector<SCL::ParameterInstance>&);

#endif
