#ifndef TIME_H_
#define TIME_H_

#include "CoreDefines.h"

#include "Types.h"

namespace Arcana
{
	class ARCANA_CORE_API Time
	{
	public:

		Time();

		Time(int64 microseconds);

		~Time();


		int64 toNanoseconds() const;

		int64 toMicroseconds() const;

		int32 toMilliseconds() const;

		double toSeconds() const;

		bool isZero() const;

		static Time fromNanoseconds(int64 nanoseconds);

		static Time fromMilliseconds(int32 milliseconds);

		static Time fromSeconds(double seconds);

	private:

		int64 _microseconds;
	};
}

#endif // !TIME_H_