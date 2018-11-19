#ifndef ENGINE_GLOBALS_H_
#define ENGINE_GLOBALS_H_

#include "Engine.h"
#include "ProfileManager.h"

#include "EngineDefines.h"

namespace Arcana
{
	extern ARCANA_ENGINE_API class Engine* GEngine;

	extern ARCANA_ENGINE_API class ProfileManager* GProfileManager;

	extern ARCANA_ENGINE_API void InitEngine();

	extern ARCANA_ENGINE_API void DestroyEngine();
}

#endif