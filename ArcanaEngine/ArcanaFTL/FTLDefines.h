#pragma once

#ifdef ARCANA_FTL_EXPORTS
#define ARCANA_FTL_API __declspec(dllexport)
#else
#define ARCANA_FTL_API __declspec(dllimport)
#endif