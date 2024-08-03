#pragma once 

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "Handlers.h"
#include "Core/Parameter.h"

static const char* const kSCLTitle = "Star Catalog Tool";
// not const so it can be changed by flag handlers for verbosity etc.
static bool kbSCLDisplayTitle = false;
static const SCL::Parameter kSCLParameters[] =
{
	{ "test" },
};

#endif
