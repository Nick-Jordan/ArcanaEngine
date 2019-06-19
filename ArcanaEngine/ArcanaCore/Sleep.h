#ifndef SLEEP_H_
#define SLEEP_H_

#include "CoreDefines.h"

#include "SleepContext.h"

namespace Arcana
{
	/** \brief Platform-independent sleep implementation. 
	 */
	class ARCANA_CORE_API Sleep
	{
	public:

		/** \brief Sleep constructor.
		 * 
		 *  Takes a Time object as an argument.
		 */
		Sleep(Time time);

		/** \brief Sleep constructor.
		 *
		 *  Takes a time in microseconds as an argument.
		 */
		Sleep(int64 microseconds);

		/** \brief Sleep constructor.
		 *
		 *  Takes a time in seconds as an argument.
		 */
		Sleep(double seconds);
	};

	/** \def AE_SLEEP(t)
	 *
	 *  Simple macro that creates a Sleep instance.
	 *  Sleeps for 't' seconds/microseconds/etc.
	 */
	#define AE_SLEEP(t) Arcana::Sleep(t)
}

#endif // !SLEEP_H_