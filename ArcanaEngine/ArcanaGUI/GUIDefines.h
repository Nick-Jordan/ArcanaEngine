#pragma once

#ifdef ARCANA_GUI_EXPORTS
#define ARCANA_GUI_API __declspec(dllexport)
#else
#define ARCANA_GUI_API __declspec(dllimport)
#endif