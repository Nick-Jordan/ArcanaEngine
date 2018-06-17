//test
#define ARCANA_PLATFORM_WINDOWS

#ifdef ARCANA_PLATFORM_WINDOWS

#ifndef WINDOWS_WINDOW_DEFINITION_H_
#define WINDOWS_WINDOW_DEFINITION_H_

#include "PlatformDefines.h"

#include "PlatformWindows.h"
#include "WindowDefinition.h"

namespace Arcana
{

	class ARCANA_PLATFORM_API WindowsWindowDefinition : public WindowDefinition
	{
	public:

		WindowsWindowDefinition();

		virtual ~WindowsWindowDefinition();


		HINSTANCE getInstance() const;

		void setInstance(HINSTANCE instance);

		int getShowCommand() const;

		void setShowCommand(int showCommand);

		const wchar_t* getWindowClass() const;

		void setWindowClass(const wchar_t* windowClass);

	private:

		HINSTANCE _instance;
		int _showCommand;
		const wchar_t* _windowClass;
	};

}

#endif // !WINDOWS_WINDOW_DEFINITION_H_

#endif // ARCANA_PLATFORM_WINDOWS

