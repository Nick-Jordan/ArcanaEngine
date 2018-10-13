#ifndef WINDOWS_SLEEP_CONTEXT_H_
#define WINDOWS_SLEEP_CONTEXT_H_

#include "CoreDefines.h"

#include "ArcanaTime.h"

#pragma comment(lib, "winmm.lib")

namespace Arcana
{
	class ARCANA_CORE_API SleepContext
	{
	public:

		static void sleep(Time time);
	};
}

#endif // !WINDOWS_SLEEP_CONTEXT_H_