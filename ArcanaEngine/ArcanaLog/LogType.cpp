#include "LogType.h"

namespace Arcana
{
	LogType::LogType() : _name("Default"), _defaultColor("#000000"), _logger(NULL)
	{
	}
		
	LogType::LogType(std::string name, std::string defaultColor, Logger* logger)
		: _name(name), _defaultColor(defaultColor), _logger(logger)
	{
		_logger->setName(name);
		_logger->setDefaultColor(defaultColor);
	}
		
	LogType::~LogType()
	{
		//delete _logger;
	}
		
	const std::string& LogType::getName()
	{
		return _name;
	}
		
	const std::string& LogType::getDefaultColor()
	{
		return _defaultColor;
	}
		
	Logger* LogType::getLogger()
	{
		return _logger;
	}
}