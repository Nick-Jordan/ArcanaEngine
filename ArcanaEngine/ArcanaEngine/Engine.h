#ifndef ENGINE_H_
#define ENGINE_H_

#include "EngineDefines.h"

#include "Object.h"
#include "Application.h"
#include "MainEngineLoop.h"
#include "Timeline.h"
#include "Callback.h"
#include "Thread.h"
#include "Timer.h"
#include "Renderer.h"

//why
#include "../ArcanaScene/World.h"

namespace Arcana
{

	class EngineInitEvent : public Event
	{
		friend class Engine;

	private:

		EngineInitEvent() : Event(EventID::DefaultID) {};

		~EngineInitEvent() {};
	};

	class EngineEventListener : public EventListener
	{
	public:

		EngineEventListener();

		virtual bool processEvent(Event& event, EventHandler& handler) override;

	public:

		bool hasFocus;
	};

	class ARCANA_ENGINE_API Engine : public Object
	{
		friend class EngineEventListener;

	public:

		Engine();

		~Engine();

		void initialize(MainEngineLoop* mainEngineLoop);

		void createApplication(const ApplicationDefinition& definition);

		void start();

		void update();

		void exit();

		Timeline& getTimeline();

		double getTotalRuntime() const;

		double getCurrentTime() const;

		Application* getApplicationInstance();

		void setRenderer(const RenderSettings& settings);

		World* getWorld();

		void setStationaryCursor(bool stationary, Vector2i position = Vector2i(-1, -1));
	
		//test
		void setWorld(World* world);

	private:

		void timelineCallback();

	private:

		MainEngineLoop* _mainEngineLoop;
	
		Application* _applicationInstance;
	
		double _maxTimeInterval;

		double _totalRuntime;

		Timeline _engineTimeline;

		Timer _timer;

		Thread* _updateThread;

		int32 _running;

		Renderer* _renderer;

		World* _world;

		bool _stationaryCursor;
		EngineEventListener* _eventListener;
		Vector2i _stationaryCursorPosition;


	};
}

#endif // !ENGINE_H_