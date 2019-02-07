#ifndef TIME_H_
#define TIME_H_

#include "CoreDefines.h"

#include "Types.h"

namespace Arcana
{
	/** \brief Class for wrapping a time duration
	 *
	 *  Stores the time in microseconds.
	 *  Provides methods for converting between time units.
	 */

	class ARCANA_CORE_API Time
	{
	public:

		/** \brief Default constructor.
		 *  
		 *  Initializes the time to zero.
		 */
		Time();

		/** \brief Int64 constructor.
		 *
		 *  Initializes the time in microseconds.
		 */
		Time(int64 microseconds);

		/** \brief Default destructor.
		 */
		~Time();


		/** \brief Returns the time in nanoseconds.
		 */
		int64 toNanoseconds() const;

		/** \brief Returns the time in microseconds.
		 */
		int64 toMicroseconds() const;

		/** \brief Returns the time in milliseconds.
		 */
		int32 toMilliseconds() const;

		/** \brief Returns the time in seconds.
		 */
		double toSeconds() const;

		/** \brief Returns true if the time is zero.
		 */
		bool isZero() const;

		/** \brief Creates a Time object from nanoseconds.
		 */
		static Time fromNanoseconds(int64 nanoseconds);

		/** \brief Creates a Time object from milliseconds.
		 */
		static Time fromMilliseconds(int32 milliseconds);

		/** \brief Creates a Time object from seconds.
		 */
		static Time fromSeconds(double seconds);

	private:

		int64 _microseconds; ///< The time in microseconds.
	};
}

#endif // !TIME_H_