#ifndef TIMER_H_
#define TIMER_H_

#include "CoreDefines.h"

#include "TimerContext.h"
#include "Time.h"

namespace Arcana
{

	class ARCANA_CORE_API Timer
	{
	public:

		Timer();
		~Timer();

		Time getElapsedTime() const;

		Time getStartTime();

		Time reset();

	private:

		Time _time;
	};

}

#endif // !TIMER_H_

