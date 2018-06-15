#pragma once

#ifdef ARCANA_PLATFORM_EXPORTS
#define ARCANA_PLATFORM_API __declspec(dllexport)
#else
#define ARCANA_PLATFORM_API __declspec(dllimport)
#endif
