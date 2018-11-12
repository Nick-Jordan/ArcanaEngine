// ArcanaLog.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ArcanaLog.h"

#include <cstdarg>
#include <cstdio>
#include <mutex>

//temp
#include <iostream>

namespace Arcana
{
	//mutex for protecting Loggers
	std::mutex logger_mutex;

	Logger::File LogOutput::File = Logger::File("arcana_output_log.html");

	INITIALIZE_CATEGORY(Arcana, CoreEngine)

	INITIALIZE_LOG_TYPE(Arcana, Debug)
	INITIALIZE_LOG_TYPE(Arcana, Warning)
	INITIALIZE_LOG_TYPE(Arcana, Error)
	INITIALIZE_LOG_TYPE(Arcana, Info)

	void LOG(Logger* customLogger, LogCategory category, const std::string& msg)
	{
		customLogger->log(category, msg);
	}
	
	void LOG(LogType type, LogCategory category, const std::string& msg)
	{		
		std::lock_guard<std::mutex> guard(logger_mutex);
		type.getLogger().log(category, msg);
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