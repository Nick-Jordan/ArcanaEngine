#include "MainEngineLoop.h"

#include <string>

namespace Arcana
{
	MainEngineLoop::MainEngineLoop()
	{
	}
		
		
	int32 MainEngineLoop::preInitialize(int32 argc, char* argv[], char* additional)
	{
		std::string commandLine;
	
		for(int32 i = 1; i < argc; i++)
		{
			std::string arg = std::string(argv[i]);
		}
	
		return preInitialize(commandLine.c_str());
	}
		
	int32 MainEngineLoop::preInitialize(const char* commandLineArgs)
	{
		return 0;
	}
	
	void MainEngineLoop::loadPreInitModules()
	{
	}
		
	void MainEngineLoop::loadCoreModules()
	{
	}
		
	bool MainEngineLoop::loadStartupCoreModules()
	{
		return false;
	}
	
	bool MainEngineLoop::loadStartupModules()
	{
		return false;
	}
		
	int32 MainEngineLoop::initialize()
	{
		return 0;
	}
		
	void MainEngineLoop::initializeTime()
	{
	}
		
	void MainEngineLoop::exit()
	{
	}
		
	//idle
	
	void MainEngineLoop::update()
	{
		//calculate elapsed time
		//update module timelines
	}
		
	void MainEngineLoop::clearPendingCleanupObjects()
	{
	}
}