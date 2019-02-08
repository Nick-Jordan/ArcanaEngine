#pragma once

/** \def ARCANA_LOG_API
 *
 *  Log DLL export define.
 */
#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif