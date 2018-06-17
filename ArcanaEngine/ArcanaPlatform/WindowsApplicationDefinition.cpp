#include "WindowsApplicationDefinition.h"

#include "WindowsWindowDefinition.h"

#ifdef ARCANA_PLATFORM_WINDOWS

namespace Arcana
{

	WindowsApplicationDefinition::WindowsApplicationDefinition() : _windowClass(L"Window")
	{
	}


	WindowsApplicationDefinition::~WindowsApplicationDefinition()
	{
	}

	void WindowsApplicationDefinition::updateWindowDefinitionData(WindowDefinition& definition)
	{
		try {
			LOG(Debug, CoreEngine, "Instance in Update: " + std::to_string((int)getInstance()));

			WindowsWindowDefinition &def = dynamic_cast<WindowsWindowDefinition&>(definition);
			LOG(Debug, CoreEngine, "Setting hInstance for window definition.");
			def.setInstance(getInstance());
			def.setShowCommand(getShowCommand());
			def.setWindowClass(getWindowClass());
		}
		catch (const std::bad_cast& e) {
		}
	}

	HINSTANCE WindowsApplicationDefinition::getInstance() const
	{
		return _instance;
	}

	void WindowsApplicationDefinition::setInstance(HINSTANCE instance)
	{
		LOG(Debug, CoreEngine, "Set Instance 1: " + std::to_string((int)instance));
		_instance = instance;
		LOG(Debug, CoreEngine, "Set Instance 2: " + std::to_string((int)_instance));
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

	const wchar_t* WindowsApplicationDefinition::getWindowClass() const
	{
		return _windowClass;
	}

	void WindowsApplicationDefinition::setWindowClass(const wchar_t* windowClass)
	{
		_windowClass = windowClass;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
