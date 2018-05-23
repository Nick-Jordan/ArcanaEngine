#ifndef MAIN_ENGINE_LOOP_H_
#define MAIN_ENGINE_LOOP_H_

#include "ArcanaCoreAPIDefinition.h"

#include "EngineLoop.h"

namespace Arcana
{
	class MainEngineLoop : public EngineLoop
	{
	public:
		
		MainEngineLoop();
		
		
		int32 preInitialize(int32 argc, char* argv[], char* additional = nullptr);
		
		int32 preInitialize(const char* commandLineArgs);
		
		void loadPreInitModules();
		
		void loadCoreModules();
		
		bool loadStartupCoreModules();
		
		bool loadStartupModules();
		
		virtual int32 initialize() override;
		
		void initializeTime();
		
		void exit();
		
		//idle
		
		virtual void update() override;
		
		virtual void clearPendingCleanupObjects() override;
	};
}

#endif