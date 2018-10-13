#include "Timer.h"

namespace Arcana
{

	Timer::Timer() : _time(0)
	{
		reset();
	}
	Timer::~Timer()
	{

	}

	Time Timer::getElapsedTime() const
	{
		return TimerContext::getCurrentTime() - _time.toMicroseconds();
	}

	Time Timer::getStartTime()
	{
		return _time;
	}

	Time Timer::reset()
	{
		int64 current = TimerContext::getCurrentTime();
		int64 elapsed = current - _time.toMicroseconds();
		_time = current;

		return elapsed;
	}

}
