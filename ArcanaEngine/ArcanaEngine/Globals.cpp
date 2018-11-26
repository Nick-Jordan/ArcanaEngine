#include "Globals.h"

#include "ArcanaLog.h"

namespace Arcana
{
	Engine* GEngine = nullptr;

	ProfileManager* GProfileManager = nullptr;

	void InitEngine()
	{
		LOG(Info, CoreEngine, "Initializing ArcanaEngine...");

		MainEngineLoop* mainEngineLoop = new MainEngineLoop();
		mainEngineLoop->preInitialize(nullptr); //<---- command line args

		GEngine = new Engine();
		GEngine->initialize(mainEngineLoop);

		//GProfileManager = new ProfileManager();
	}

	void DestroyEngine()
	{
		//free engine resources that still exist

		//test file
		//GProfileManager->exportSamples("profile_output.csv");

		//AE_DELETE(GProfileManager);
		AE_DELETE(GEngine);
	}
}