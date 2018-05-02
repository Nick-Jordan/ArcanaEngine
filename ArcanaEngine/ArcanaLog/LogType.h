#ifndef LOG_TYPE_H_
#define LOG_TYPE_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include "Logger.h"

namespace Arcana
{
	class ARCANA_LOG_API LogType
	{
	public:
		
		LogType();
		
		LogType(std::string name, std::string defaultColor, Logger* logger);
		
		~LogType();
		
		const std::string& getName();
		
		const std::string& getDefaultColor();
		
		Logger* getLogger();
	
	private:
	
		std::string _name;
		std::string _defaultColor;
		Logger* _logger;
	};
}

#endif //!LOG_TYPE_H_