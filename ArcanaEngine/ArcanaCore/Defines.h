#pragma once

#include <assert.h>

#ifdef _DEBUG
#define AE_ASSERT(expr) assert(expr)
#else
#define AE_ASSERT(expr)
#endif
