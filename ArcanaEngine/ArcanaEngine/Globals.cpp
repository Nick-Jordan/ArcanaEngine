#include "Globals.h"

#include "ArcanaLog.h"

namespace Arcana
{
	Engine* GEngine = nullptr;

	void InitEngine()
	{
		LOG(Info, CoreEngine, "Initializing ArcanaEngine...");

		MainEngineLoop mainEngineLoop;
		mainEngineLoop.preInitialize(nullptr); //<---- command line args

		GEngine = new Engine();
		GEngine->initialize(&mainEngineLoop);
	}

	void DestroyEngine()
	{
		//free engine resources that still exist

		AE_DELETE(GEngine);
	}
}