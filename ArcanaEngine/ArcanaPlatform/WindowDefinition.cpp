#include "WindowDefinition.h"

namespace Arcana
{
	WindowDefinition::WindowDefinition() :
		_title("Default Window"),
		_width(0),
		_height(0),
		_style(Default)
	{

	}

	WindowDefinition::~WindowDefinition()
	{

	}


	const std::string& WindowDefinition::getTitle() const
	{
		return _title;
	}

	void WindowDefinition::setTitle(const std::string& title)
	{
		_title = title;
	}

	const uint32& WindowDefinition::getWidth() const
	{
		return _width;
	}

	void WindowDefinition::setWidth(uint32 width)
	{
		_width = width;
	}

	const uint32& WindowDefinition::getHeight() const
	{
		return _height;
	}

	void WindowDefinition::setHeight(uint32 height)
	{
		_height = height;
	}

	uint32 WindowDefinition::getStyle() const
	{
		return _style;
	}

	void WindowDefinition::setStyle(uint32 style)
	{
		_style = style;
	}
}
