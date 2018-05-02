#ifndef LOG_DEFINES_H_
#define LOG_DEFINES_H_

#include "LogCategory.h"
#include "LogType.h"

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#define ADD_LOG_FILE(Name, FileName) \
	ARCANA_LOG_API struct LogFile##Name : public Arcana::Logger::File \
	{ \
		LogFile##Name() : Arcana::Logger::File(#FileName) {} \
	} *Name = new LogFile##Name();

#define REGISTER_CATEGORY(CategoryName, CategoryHexColor) \
	ARCANA_LOG_API struct Log##CategoryName : public Arcana::LogCategory \
	{ \
		Log##CategoryName() : Arcana::LogCategory(#CategoryName, #CategoryHexColor) {} \
	} CategoryName;
	

#define REGISTER_LOG_TYPE(TypeName, DefaultHexColor, File) \
	ARCANA_LOG_API struct Log##TypeName : public Arcana::LogType \
	{ \
		Log##TypeName() : Arcana::LogType(#TypeName, #DefaultHexColor, new Arcana::Logger(File)) {} \
	} TypeName;

//ADD_LOG_FILE(defaultLogFile, log.html)

ARCANA_LOG_API struct LogFiledefaultLogFile : public Arcana::Logger::File \
	{ \
		LogFiledefaultLogFile() : Arcana::Logger::File("log.html") {} \
	} *defaultLogFile = new LogFiledefaultLogFile();

REGISTER_CATEGORY(Engine, none)

REGISTER_LOG_TYPE(Debug, #888888, defaultLogFile)
REGISTER_LOG_TYPE(Warning, #CC9900, defaultLogFile)
REGISTER_LOG_TYPE(Error, #CC0000, defaultLogFile)
REGISTER_LOG_TYPE(Info, #0000FF, defaultLogFile)


#endif //!LOG_DEFINES_H_