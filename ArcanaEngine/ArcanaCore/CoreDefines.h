#pragma once

#include <assert.h>

/** \def ARCANA_CORE_API
 *
 *  Core DLL export define.
 */
#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

/** \def AE_ASSERT(expr)
 *
 *  ArcanaEngine assertion.
 *  If the program is running in debug mode, it performs a normal assert.
 *  Otherwise it does nothing.
 */
#ifdef _DEBUG
#define AE_ASSERT(expr) assert(expr)
#else
#define AE_ASSERT(expr)
#endif

/** \def AE_DELETE(x)
 *
 *  Safe delete.
 *  Evaluates the parameter before deleting.
 */
#define AE_DELETE(x) \
    { \
        delete x; \
        x = nullptr; \
    }

/** \def AE_DELETE_ARRAY(x)
 *
 *  Safe array delete.
 *  Evaluates the parameter before deleting.
 */
#define AE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = nullptr; \
    }

/** \def AE_RELEASE(x)
 *
 *  Releases an object.
 *  Parameter must be an instance of Object.
 *  Used for reference counting.
 */
#define AE_RELEASE(x) \
    if (x) \
    { \
        (x)->release(); \
        x = nullptr; \
    }
