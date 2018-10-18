#pragma once

#ifdef ARCANA_SCENE_EXPORTS
#define ARCANA_SCENE_API __declspec(dllexport)
#else
#define ARCANA_SCENE_API __declspec(dllimport)
#endif