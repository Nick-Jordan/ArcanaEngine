// GraphicsModule.cpp : Defines the exported functions for the DLL application.
//

#include "GraphicsModule.h"

namespace Arcana
{
	GraphicsModule::GraphicsModule()
	{

	}

	GraphicsModule::~GraphicsModule()
	{

	}

	bool GraphicsModule::startUp()
	{
		return true;
	}

	bool GraphicsModule::shutDown()
	{
		return true;
	}

	bool GraphicsModule::isGameModule()
	{
		return false;
	}
}


