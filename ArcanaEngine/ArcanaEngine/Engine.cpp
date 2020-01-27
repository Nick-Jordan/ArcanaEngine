#include "Engine.h"
#include "ArcanaLog.h"
#include "Sleep.h"
#include "Input.h"

#include "GlobalShaders.h"
#include "InputComponent.h"

#include "ResourceManager.h"

namespace Arcana
{
	Engine::Engine() : Object("Engine"), _running(0), _totalRuntime(0.0), _renderer(nullptr), _world(nullptr), _stationaryCursor(false), 
		_applicationInstance(nullptr), _mainEngineLoop(nullptr), _updateThread(nullptr)
	{
		LOG(Debug, CoreEngine, "Engine created");

		_engineTimeline.setTimelineLengthMode(Timeline::LastKeyFrame);
		_engineTimeline.addEvent(1.0, new EngineInitEvent());
		_engineTimeline.getTimelineFinishedCallback().bind(this, &Engine::timelineCallback);
	}

	Engine::~Engine()
	{
		_engineTimeline.stop();

		AE_DELETE(_applicationInstance);

		AE_DELETE(_mainEngineLoop);

		AE_DELETE(_updateThread);

		AE_DELETE(_renderer);
	}

	void Engine::initialize(MainEngineLoop* mainEngineLoop)
	{
		_mainEngineLoop = mainEngineLoop;

		int32 error = mainEngineLoop->initialize();


	}

	void Engine::createApplication(const ApplicationDefinition& definition)
	{
		_applicationInstance = new Application(definition);

		_eventListener = new EngineEventListener();

		_applicationInstance->getEventHandler().addEventListener(std::shared_ptr<EngineEventListener>(_eventListener));

		InputComponent::createInputComponentManager();

		if (InputComponent::Manager)
		{
			_applicationInstance->getEventHandler().addEventListener(std::shared_ptr<InputComponentManager>(InputComponent::Manager));
		}
	}

	void Engine::start()
	{
		_running = 1;

		_updateThread = new Thread(&Engine::update, this);

		if (_applicationInstance)
		{
			//_applicationInstance->setUpdater(EngineLoopUpdater(_mainEngineLoop));
			_engineTimeline.setEventHandler(_applicationInstance->getEventHandler());
		}

		_engineTimeline.play();

		if (_world)
		{
			for (uint32 i = 0; i < _world->getNumActors(); i++)
			{
				Actor* actor = _world->getActor(i);

				if (actor)
				{
					actor->begin();
				}
			}
		}

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

			//LOGF(Error, CoreEngine, "Elapsed Time: %f", elapsedTime);
			//LOGF(Error, CoreEngine, "FPS: %f", 1.0 / elapsedTime);
			//LOGF(Info, CoreEngine, "Engine Timeline: %f", _engineTimeline.getPlaybackPosition());
			//LOGF(Info, CoreEngine, "Current Engine Time: %f", getCurrentTime());

			ResourceManager::instance().finalizePendingResources();

			if (_stationaryCursor && _eventListener->hasFocus)
			{
				Input::setMousePosition(_stationaryCursorPosition);
			}

			if (_world)
			{
				_world->updateActors(elapsedTime);
			}
			_engineTimeline.updateTimeline(elapsedTime);

			//update world/objects

			if (_mainEngineLoop)
			{
				_mainEngineLoop->clearPendingCleanupObjects();
			}

			double updateLimit = 0.0;// 1.0 / 60.0;

			if(updateLimit != 0.0)
			{
				AE_SLEEP(updateLimit - elapsedTime);
			}
		}
	}

	void Engine::exit()
	{
		_running = 0;

		_mainEngineLoop->shutdownModules();
		_mainEngineLoop->exit();

		//cleanup world/objects

		if (_world)
		{
			for (uint32 i = 0; i < _world->getNumActors(); i++)
			{
				Actor* actor = _world->getActor(i);

				if (actor)
				{
					actor->end();
					actor->destroy();
					actor->allowDestruction();
				}
			}

			_mainEngineLoop->clearPendingCleanupObjects();

			AE_RELEASE(_world);
		}
	}

	Timeline& Engine::getTimeline()
	{
		return _engineTimeline;
	}

	double Engine::getTotalRuntime() const
	{
		return _totalRuntime;
	}

	double Engine::getCurrentTime() const
	{
		return _totalRuntime + _engineTimeline.getPlaybackPosition();
	}

	Application* Engine::getApplicationInstance()
	{
		return _applicationInstance;
	}

	void Engine::setRenderer(const RenderSettings& settings)
	{
		if (_applicationInstance)
		{
			_renderer = new Renderer(settings, &_applicationInstance->getActiveWindow());
		}
		else
		{
			_renderer = new Renderer(settings, 1920, 1080);//test (change width/height)
		}

		if (_world)
		{
			_renderer->setWorldRenderer(_world);
		}

		if (!GlobalShaders::initialize())
		{
			LOG(Warning, CoreEngine, "Not all global shaders were properly initialized! This could cause crashes!");
		}
	}

	World* Engine::getWorld()
	{
		return _world;
	}

	void Engine::setStationaryCursor(bool stationary, Vector2i position)
	{
		_stationaryCursor = stationary;

		if (stationary)
		{
			if (position.x == -1 && position.y == -1 && _applicationInstance)
			{
				position = _applicationInstance->getActiveWindow().getSize() / 2;
			}

			_stationaryCursorPosition = position;
		}
	}

	Vector2i Engine::getStationaryCursorPosition() const
	{
		return _stationaryCursorPosition;
	}

	void Engine::setWorld(World* world)
	{
		_world = world;

		if (_world)
		{
			_world->reference();
			_renderer->setWorldRenderer(_world);

			if (_running)
			{
				for (uint32 i = 0; i < _world->getNumActors(); i++)
				{
					Actor* actor = _world->getActor(i);

					if (actor)
					{
						actor->begin();
					}
				}
			}
		}
	}

	EngineEventListener::EngineEventListener() : hasFocus(false)
	{
		listenForEvent(EventID::WindowFocusEventID);
	}

	bool EngineEventListener::processEvent(Event& event, EventHandler& handler)
	{
		if (event.getEventId() == EventID::WindowFocusEventID)
		{
			hasFocus = event.getInt("type") == 1;
		}

		return true;
	}

	void Engine::timelineCallback()
	{
		//LOGF(Info, CoreEngine, "Timeline Finished Callback Called: Total runtime = %f", getTotalRuntime());

		_totalRuntime += _engineTimeline.getTimelineLength();

		_engineTimeline.setPlaybackPosition(0.0, false);
		if (!_engineTimeline.isPlaying())
		{
			_engineTimeline.play();
		}
	}
}
