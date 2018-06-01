#pragma once

#ifdef ARCANA_INPUT_EXPORTS
#define ARCANA_INPUT_API __declspec(dllexport)
#else
#define ARCANA_INPUT_API __declspec(dllimport)
#endif