#include "stdafx.h"

#include "Engine.h"
#include "ArcanaLog.h"
#include "Sleep.h"

namespace Arcana
{
	Engine::Engine() : Object("Engine"), _running(0)
	{
		LOG(Debug, CoreEngine, "Engine created");

		_engineTimeline.setTimelineLengthMode(Timeline::LastKeyFrame);
	}

	Engine::~Engine()
	{
		_engineTimeline.stop();

		if (_applicationInstance)
		{
			AE_DELETE(_applicationInstance)
		}

		if (_updateThread)
		{
			AE_DELETE(_updateThread)
		}
	}

	void Engine::initialize(MainEngineLoop* mainEngineLoop)
	{
		_mainEngineLoop = mainEngineLoop;

		int32 error = mainEngineLoop->initialize();


	}

	void Engine::createApplication(const ApplicationDefinition& definition)
	{
		_applicationInstance = new Application(definition);
	}

	void Engine::start()
	{
		_running = 1;

		_updateThread = new Thread(&Engine::update, this);

		if (_applicationInstance)
		{
			//_applicationInstance->setUpdater(EngineLoopUpdater(_mainEngineLoop));
		}

		_engineTimeline.play();

		_updateThread->start();
		if (_applicationInstance)
		{
			_applicationInstance->start();
		}
	}

	void Engine::update()
	{
		while (_running)
		{
			double elapsedTime = _timer.reset().toSeconds();

			LOGF(Error, CoreEngine, "Elapsed Time: %f", elapsedTime);
			LOGF(Error, CoreEngine, "FPS: %f", 1.0 / elapsedTime);
			LOGF(Info, CoreEngine, "Engine Timeline: %f", _engineTimeline.getPlaybackPosition());

			_engineTimeline.updateTimeline(elapsedTime);

			//update world/objects

			double updateLimit = 1.0 / 60.0;

			if(updateLimit != 0.0)
			{
				AE_SLEEP(updateLimit - elapsedTime)
			}
		}
	}

	void Engine::exit()
	{
		_running = 0;

		_mainEngineLoop->shutdownModules();

		//cleanup world/objects
	}

	Timeline& Engine::getTimeline()
	{
		return _engineTimeline;
	}
}
