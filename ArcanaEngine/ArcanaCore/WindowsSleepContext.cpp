#include "WindowsSleepContext.h"

#include <Windows.h>
#include <timeapi.h>

namespace Arcana
{
	void SleepContext::sleep(Time time)
	{
		TIMECAPS tc;
		timeGetDevCaps(&tc, sizeof(TIMECAPS));
		timeBeginPeriod(tc.wPeriodMin);

		Sleep(time.toMilliseconds());

		timeEndPeriod(tc.wPeriodMin);
	}
}