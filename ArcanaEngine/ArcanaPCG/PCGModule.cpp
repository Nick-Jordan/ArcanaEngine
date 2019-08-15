// PCGModule.cpp : Defines the exported functions for the DLL application.
//

#include "PCGModule.h"

namespace Arcana
{
	PCGModule::PCGModule()
	{

	}

	PCGModule::~PCGModule()
	{

	}

	bool PCGModule::startUp()
	{
		return true;
	}

	bool PCGModule::shutDown()
	{
		return true;
	}

	bool PCGModule::isGameModule()
	{
		return false;
	}

	const char* PCGModule::getModuleName() const
	{
		return "PCG";
	}
}


