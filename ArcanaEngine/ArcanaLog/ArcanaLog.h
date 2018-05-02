#ifndef ARCANA_LOG_H_
#define ARCANA_LOG_H_

#define ARCANA_LOG_EXPORTS

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include "LogDefines.h"

namespace Arcana
{
	extern ARCANA_LOG_API void LOG(Logger* customLogger, LogCategory category, const std::string& msg);
	
	extern ARCANA_LOG_API void LOG(LogType type, LogCategory category, const std::string& msg);
	
	extern ARCANA_LOG_API void LOGF(Logger* customLogger, LogCategory category, const char* msg, ...);
	
	extern ARCANA_LOG_API void LOGF(LogType type, LogCategory category, const char* msg, ...);
	
}

#endif //!ARCANA_LOG_H_