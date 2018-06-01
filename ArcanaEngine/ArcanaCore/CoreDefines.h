#pragma once

#include <assert.h>

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#define AE_ASSERT(expr) assert(expr)
#else
#define AE_ASSERT(expr)
#endif
