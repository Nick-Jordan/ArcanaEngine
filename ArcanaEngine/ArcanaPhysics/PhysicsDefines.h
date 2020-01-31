#pragma once

#ifdef ARCANA_PHYSICS_EXPORTS
#define ARCANA_PHYSICS_API __declspec(dllexport)
#else
#define ARCANA_PHYSICS_API __declspec(dllimport)
#endif