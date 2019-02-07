#ifndef WINDOWS_TIMER_CONTEXT_H_
#define WINDOWS_TIMER_CONTEXT_H_

#include "CoreDefines.h"

#include "Types.h"
#include <Windows.h>

namespace Arcana
{
	/** \brief Windows timer implementation
	 */
	class ARCANA_CORE_API TimerContext
	{
	public:
		
		/** \brief Returns the current time in microseconds
		 */
		static int64 getCurrentTime();
	};

}

#endif // !WINDOWS_TIMER_CONTEXT_H_

