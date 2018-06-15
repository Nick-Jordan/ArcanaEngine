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

		std::string someData;
	};

}

#endif // !WINDOWS_WINDOW_DEFINITION_H_

#endif // ARCANA_PLATFORM_WINDOWS

