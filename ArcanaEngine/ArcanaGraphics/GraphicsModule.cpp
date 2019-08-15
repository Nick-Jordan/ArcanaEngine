// GraphicsModule.cpp : Defines the exported functions for the DLL application.
//

#include "GraphicsModule.h"

#include "PostProcessor.h"

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
		PostProcessor::initialize();

		return true;
	}

	bool GraphicsModule::shutDown()
	{
		PostProcessor::finalize();

		return true;
	}

	bool GraphicsModule::isGameModule()
	{
		return false;
	}

	const char* GraphicsModule::getModuleName() const
	{
		return "Graphics";
	}
}


