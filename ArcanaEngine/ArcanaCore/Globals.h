#ifndef ENGINE_GLOBALS_H_
#define ENGINE_GLOBALS_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#include "Engine.h"

namespace Arcana
{
	extern ARCANA_CORE_API class Engine* ArcanaEngine;
}

#endif