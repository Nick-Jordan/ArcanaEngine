#include "WindowsApplicationDefinition.h"

#ifdef ARCANA_PLATFORM_WINDOWS

namespace Arcana
{

	WindowsApplicationDefinition::WindowsApplicationDefinition()
	{
	}


	WindowsApplicationDefinition::~WindowsApplicationDefinition()
	{
	}

	HINSTANCE WindowsApplicationDefinition::getInstance() const
	{
		return _instance;
	}

	void WindowsApplicationDefinition::setInstance(HINSTANCE instance)
	{
		_instance = instance;
	}

	LPWSTR WindowsApplicationDefinition::getCommandLineArgs() const
	{
		return _commandLineArgs;
	}

	void WindowsApplicationDefinition::setCommandLineArgs(LPWSTR commandLineArgs)
	{
		_commandLineArgs = commandLineArgs;
	}

	int WindowsApplicationDefinition::getShowCommand() const
	{
		return _showCommand;
	}

	void WindowsApplicationDefinition::setShowCommand(int showCommand)
	{
		_showCommand = showCommand;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
