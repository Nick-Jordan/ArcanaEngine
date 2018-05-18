#ifndef ENGINE_GLOBALS_H_
#define ENGINE_GLOBALS_H_

#undef ARCANA_CORE_EXPORTS

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#include "Engine.h"

namespace Arcana
{
	extern __declspec(dllimport) class Engine* GEngine;
}

#define ARCANA_CORE_EXPORTS

#endif