#ifndef LOG_DEFINES_H_
#define LOG_DEFINES_H_

#include "LogType.h"

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

REGISTER_CATEGORY(Engine, none)

REGISTER_LOG_TYPE(Debug, #888888)
REGISTER_LOG_TYPE(Warning, #CC9900)
REGISTER_LOG_TYPE(Error, #CC0000)
REGISTER_LOG_TYPE(Info, #0000FF)


#endif //!LOG_DEFINES_H_