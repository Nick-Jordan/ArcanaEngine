#pragma once

#ifdef ARCANA_PCG_EXPORTS
#define ARCANA_PCG_API __declspec(dllexport)
#else
#define ARCANA_PCG_API __declspec(dllimport)
#endif