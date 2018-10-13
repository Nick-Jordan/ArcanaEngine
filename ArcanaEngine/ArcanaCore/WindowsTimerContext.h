#ifndef WINDOWS_TIMER_CONTEXT_H_
#define WINDOWS_TIMER_CONTEXT_H_

#include "CoreDefines.h"

#include "Types.h"
#include <Windows.h>

namespace Arcana
{

	class ARCANA_CORE_API TimerContext
	{
	public:
		
		static int64 getCurrentTime();
	};

}

#endif // !WINDOWS_TIMER_CONTEXT_H_

