#include "WindowsTimerContext.h"

namespace Arcana
{
	LARGE_INTEGER getFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency;
	}

	int64 TimerContext::getCurrentTime()
	{
		static LARGE_INTEGER frequency = getFrequency();

		LARGE_INTEGER time;

		QueryPerformanceCounter(&time);
		
		return 1000000 * time.QuadPart / frequency.QuadPart;
	}

}
