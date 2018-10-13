#include "MainEngineLoop.h"

#include <string>

//Modules
#include "CoreModule.h"
#include "EngineModule.h"
#include "GraphicsModule.h"
#include "InputModule.h"

namespace Arcana
{
	MainEngineLoop::MainEngineLoop()
	{
	}
		
	MainEngineLoop::~MainEngineLoop()
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

	void MainEngineLoop::preInitModules()
	{
		_initQueue.add(new CoreModule());
		_initQueue.add(new GraphicsModule());
		_initQueue.add(new InputModule());
		_initQueue.add(new EngineModule());
	}

	int32 MainEngineLoop::startupModules()
	{
		int32 error = 0;

		int32 index = 1;
		for (auto i = _initQueue.createConstIterator(); i; i++)
		{
			bool err = (*i)->startUp();

			if (err)
			{
				error &= 1 << index;
			}

			index++;
		}

		return error;
	}

	int32 MainEngineLoop::shutdownModules()
	{
		int32 error = 0;

		for (int32 index = _initQueue.size() - 1; index >= 0; index--)
		{
			bool err = _initQueue[index]->shutDown();

			if (err)
			{
				error &= 1 << (index + 1);
			}
		}

		return error;
	}
		
	int32 MainEngineLoop::initialize()
	{
		preInitModules();

		int32 moduleStartup = startupModules();

		initializeTime();

		return moduleStartup;
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

		LOG(Info, CoreEngine, "MAIN ENGINE LOOP UPDATING FROM APPLICATION UPDATE");
	}
		
	void MainEngineLoop::clearPendingCleanupObjects()
	{
	}
}