#pragma once

#ifdef ARCANA_GRAPHICS_EXPORTS
#define ARCANA_GRAPHICS_API __declspec(dllexport)
#else
#define ARCANA_GRAPHICS_API __declspec(dllimport)
#endif
