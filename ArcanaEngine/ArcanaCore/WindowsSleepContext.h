#ifndef WINDOWS_SLEEP_CONTEXT_H_
#define WINDOWS_SLEEP_CONTEXT_H_

#include "CoreDefines.h"

#include "ArcanaTime.h"

#pragma comment(lib, "winmm.lib")

namespace Arcana
{
	/** \brief Windows sleep implementation.
	 */
	class ARCANA_CORE_API SleepContext
	{
	public:

		/** \brief Sleeps for 'time'
		 */
		static void sleep(Time time);
	};
}

#endif // !WINDOWS_SLEEP_CONTEXT_H_