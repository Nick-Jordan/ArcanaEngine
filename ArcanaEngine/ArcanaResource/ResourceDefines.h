#pragma once

#ifdef ARCANA_RESOURCE_EXPORTS
#define ARCANA_RESOURCE_API __declspec(dllexport)
#else
#define ARCANA_RESOURCE_API __declspec(dllimport)
#endif