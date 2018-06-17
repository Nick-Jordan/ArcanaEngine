#include "WindowsWindowDefinition.h"

#ifdef ARCANA_PLATFORM_WINDOWS

namespace Arcana
{

	WindowsWindowDefinition::WindowsWindowDefinition() : WindowDefinition()
	{
	}


	WindowsWindowDefinition::~WindowsWindowDefinition()
	{
	}

	HINSTANCE WindowsWindowDefinition::getInstance() const
	{
		return _instance;
	}

	void WindowsWindowDefinition::setInstance(HINSTANCE instance)
	{
		_instance = instance;
	}

	int WindowsWindowDefinition::getShowCommand() const
	{
		return _showCommand;
	}

	void WindowsWindowDefinition::setShowCommand(int showCommand)
	{
		_showCommand = showCommand;
	}

	const wchar_t* WindowsWindowDefinition::getWindowClass() const
	{
		return _windowClass;
	}

	void WindowsWindowDefinition::setWindowClass(const wchar_t* windowClass)
	{
		_windowClass = windowClass;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
