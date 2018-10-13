#include "Time.h"

namespace Arcana
{
	Time::Time() : _microseconds(0)
	{

	}

	Time::Time(int64 microseconds) : _microseconds(microseconds)
	{

	}

	Time::~Time()
	{

	}


	int64 Time::toNanoseconds() const
	{
		return _microseconds * 1000;
	}

	int64 Time::toMicroseconds() const
	{
		return _microseconds;
	}

	int32 Time::toMilliseconds() const
	{
		return (int32)(_microseconds / 1000);
	}

	double Time::toSeconds() const
	{
		return (double)_microseconds / (1000000.0);
	}

	bool Time::isZero() const
	{
		return _microseconds == 0;
	}

	Time Time::fromNanoseconds(int64 nanoseconds)
	{
		return Time(nanoseconds / 1000);
	}

	Time Time::fromMilliseconds(int32 milliseconds)
	{
		return Time((int64)(milliseconds * 1000));
	}

	Time Time::fromSeconds(double seconds)
	{
		return Time((int64)(seconds * 1000000.0));
	}
}