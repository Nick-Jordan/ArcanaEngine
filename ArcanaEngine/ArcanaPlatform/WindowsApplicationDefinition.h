//test
#define ARCANA_PLATFORM_WINDOWS

#ifdef ARCANA_PLATFORM_WINDOWS

#ifndef WINDOWS_APPLICATION_DEFINITION_H_
#define WINDOWS_APPLICATION_DEFINITION_H_

#include "PlatformDefines.h"

#include "ApplicationDefinition.h"

#include <Windows.h>

namespace Arcana
{

	class ARCANA_PLATFORM_API WindowsApplicationDefinition : public ApplicationDefinition
	{
	public:

		WindowsApplicationDefinition();

		virtual ~WindowsApplicationDefinition();


		HINSTANCE getInstance() const;

		void setInstance(HINSTANCE instance);

		/*HINSTANCE getPrevInstance() const;

		void setPrevInstance(HINSTANCE instance);*/

		LPWSTR getCommandLineArgs() const;

		void setCommandLineArgs(LPWSTR commandLineArgs);

		int getShowCommand() const;

		void setShowCommand(int showCommand);

	private:

		HINSTANCE _instance;
		//HINSTANCE _prevInstance;
		LPWSTR _commandLineArgs;
		int _showCommand;
	};
}

#endif // !WINDOWS_APPLICATION_DEFINITION_H_

#endif // ARCANA_PLATFORM_WINDOWS

