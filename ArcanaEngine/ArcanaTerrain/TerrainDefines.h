#pragma once

#ifdef ARCANA_TERRAIN_EXPORTS
#define ARCANA_TERRAIN_API __declspec(dllexport)
#else
#define ARCANA_TERRAIN_API __declspec(dllimport)
#endif