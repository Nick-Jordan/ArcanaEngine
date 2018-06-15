#ifndef WINDOW_HANDLE_H_
#define WINDOW_HANDLE_H_

namespace Arcana
{

#ifdef ARCANA_PLATFORM_WINDOWS
	#include "PlatformWindows.h"
	typedef HWND WindowHandle;
#else
	typedef unsigned long WindowHandle;
#endif

//add other platforms with elif

}

#endif // !WINDOW_HANDLE_H_