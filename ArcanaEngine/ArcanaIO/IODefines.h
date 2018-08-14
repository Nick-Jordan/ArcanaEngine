#pragma once

#ifdef ARCANA_IO_EXPORTS
#define ARCANA_IO_API __declspec(dllexport)
#else
#define ARCANA_IO_API __declspec(dllimport)
#endif