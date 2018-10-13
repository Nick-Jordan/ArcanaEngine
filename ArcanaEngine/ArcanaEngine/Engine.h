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
	
	private:

		MainEngineLoop* _mainEngineLoop;
	public: //test
		Application* _applicationInstance;
	private://test
		double _maxTimeInterval;

		Timeline _engineTimeline;

		Timer _timer;

		Thread* _updateThread;

		int32 _running;
	};
}

#endif // !ENGINE_H_