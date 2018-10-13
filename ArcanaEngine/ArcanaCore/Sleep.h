#ifndef SLEEP_H_
#define SLEEP_H_

#include "CoreDefines.h"

#include "SleepContext.h"

namespace Arcana
{

#define AE_SLEEP(t) Arcana::Sleep(t);

	class ARCANA_CORE_API Sleep
	{
	public:

		Sleep(Time time);

		Sleep(int64 microseconds);

		Sleep(double seconds);
	};
}

#endif // !SLEEP_H_