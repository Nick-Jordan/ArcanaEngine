#pragma once

#include <assert.h>

/** \def Core DLL export define.
 */
#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

/** \def ArcanaEngine assertion.
 *
 *  If the program is running in debug mode, it performs a normal assert.
 *  Otherwise it does nothing.
 */
#ifdef _DEBUG
#define AE_ASSERT(expr) assert(expr)
#else
#define AE_ASSERT(expr)
#endif

/** \def Safe delete.
 *
 *  Evaluates the parameter before deleting.
 */
#define AE_DELETE(x) \
    { \
        delete x; \
        x = nullptr; \
    }

/** \def Safe array delete.
 *
 *  Evaluates the parameter before deleting.
 */
#define AE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = nullptr; \
    }

/** \def Releases an object.
 *
 *  Parameter must be an instance of Object.
 *  Used for reference counting.
 */
#define AE_RELEASE(x) \
    if (x) \
    { \
        (x)->release(); \
        x = nullptr; \
    }
