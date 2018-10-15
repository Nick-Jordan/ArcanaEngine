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

namespace Arcana
{

	class EngineInitEvent : public Event
	{
		friend class Engine;

	private:

		EngineInitEvent() : Event(EventID::DefaultID) {};

		~EngineInitEvent() {};
	};

	class ARCANA_ENGINE_API Engine : public Object
	{
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

		Application* getApplicationInstance();
	
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
	};
}

#endif // !ENGINE_H_