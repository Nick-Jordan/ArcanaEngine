#include "LogCategory.h"

namespace Arcana
{
	LogCategory::LogCategory() : _name("Default"), _hexColor("none")
	{
	}
		
	LogCategory::LogCategory(std::string name, std::string hexColor) : _name(name), _hexColor(hexColor)
	{
	}
		
	LogCategory::~LogCategory()
	{
	}
		
	const std::string& LogCategory::getName()
	{
		return _name;
	}
		
	const std::string& LogCategory::getHexColor()
	{
		return _hexColor;
	}
}