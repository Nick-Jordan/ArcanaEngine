#pragma once

#ifdef ARCANA_SOUND_EXPORTS
#define ARCANA_SOUND_API __declspec(dllexport)
#else
#define ARCANA_SOUND_API __declspec(dllimport)
#endif