#include "Sleep.h"

namespace Arcana
{
	Sleep::Sleep(Time time)
	{
		if (!time.isZero())
		{
			SleepContext::sleep(time);
		}
	}

	Sleep::Sleep(int64 microseconds)
	{
		if (microseconds > 0)
		{
			SleepContext::sleep(Time(microseconds));
		}
	}

	Sleep::Sleep(double seconds)
	{
		if (seconds > 0.0)
		{
			SleepContext::sleep(Time::fromSeconds(seconds));
		}
	}
}