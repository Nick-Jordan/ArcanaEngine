#include "LogType.h"

#include "ArcanaLog.h"

namespace Arcana
{
	LogType::LogType() : _name("Default"), _defaultColor("#000000"), _logger(Logger())
	{

	}
		
	LogType::LogType(std::string name, std::string defaultColor, Logger& logger)
		: _name(name), _defaultColor(defaultColor), _logger(logger)
	{
		_logger.setName(name);
		_logger.setDefaultColor(defaultColor);

		_logger.reference();
	}

	LogType::LogType(const LogType& type)
		: _name(type._name), _defaultColor(type._defaultColor), _logger(type._logger)
	{
		_logger.reference();
	}
		
	LogType::~LogType()
	{
		_logger.release();
	}
		
	const std::string& LogType::getName()
	{
		return _name;
	}
		
	const std::string& LogType::getDefaultColor()
	{
		return _defaultColor;
	}
		
	Logger& LogType::getLogger()
	{
		return _logger;
	}
}