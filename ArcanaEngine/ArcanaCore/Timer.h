#ifndef TIMER_H_
#define TIMER_H_

#include "CoreDefines.h"

#include "TimerContext.h"
#include "ArcanaTime.h"

namespace Arcana
{
	/** \brief A basic timer.
	 *
	 *  Calculates the elapsed time.
	 */
	class ARCANA_CORE_API Timer
	{
	public:

		/** \brief Timer default constructor.
		 */
		Timer();

		/** \brief Timer destructor.
		 */
		~Timer();

		/** \brief Returns the elapsed time and resets the clock.
		 */
		Time getElapsedTime() const;

		/** \brief Returns the clock's start time.
		 */
		Time getStartTime();

		/** \brief Resets the clock.
		 */
		Time reset();

	private:

		Time _time;  ///< The clock's start time.
	};

}

#endif // !TIMER_H_

