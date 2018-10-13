#pragma once

#ifdef ARCANA_ENGINE_EXPORTS
#define ARCANA_ENGINE_API __declspec(dllexport)
#else
#define ARCANA_ENGINE_API __declspec(dllimport)
#endif
