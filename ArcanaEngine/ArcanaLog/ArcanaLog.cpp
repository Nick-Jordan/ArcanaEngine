// ArcanaLog.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ArcanaLog.h"

#include <cstdarg>
#include <cstdio>

//temp
#include <iostream>

namespace Arcana
{
	void LOG(Logger* customLogger, LogCategory category, const std::string& msg)
	{
		std::cout << msg << std::endl;
	}
	
	void LOG(LogType type, LogCategory category, const std::string& msg)
	{		
		type.getLogger()->log(category, msg);
	}
	
	void LOGF(Logger* customLogger, LogCategory category, const char* msg, ...)
	{
		static const int MAX_LOG_SIZE = 512;
		char buf[MAX_LOG_SIZE];

		va_list vl;
		va_start(vl, msg);
		vsnprintf(buf, MAX_LOG_SIZE, msg, vl);
		va_end(vl);
		std::string msgStr = buf;
		LOG(customLogger, category, msgStr);
	}
	
	void LOGF(LogType type, LogCategory category, const char* msg, ...)
	{
		static const int MAX_LOG_SIZE = 512;
		char buf[MAX_LOG_SIZE];

		va_list vl;
		va_start(vl, msg);
		vsnprintf(buf, MAX_LOG_SIZE, msg, vl);
		va_end(vl);
		std::string msgStr = buf;
		LOG(type, category, msgStr);
	}
}