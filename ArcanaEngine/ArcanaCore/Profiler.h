#ifndef PROFILER_H_
#define PROFILER_H_

#include "CoreDefines.h"

#include "Types.h"

namespace Arcana
{
	/** \brief Times a section of code and stores the sample with the ProfileManager.
	 */
	class ARCANA_CORE_API AutoProfile 
	{
	public:

		/** \brief AutoProfile constructor.
		 *
		 *  Starts a timer for a section of code.
		 */
		AutoProfile(const char* name);

		/** \brief AutoProfile destructor.
		 *
		 *  Stops the tile and stores the sample.
		 */
		~AutoProfile();

	private:

		const char* _name;  ///< The name of the section of code.
		int64 _startTime;   ///< The initial time.
	};
	
	/** \def PROFILE(name)
	 *
	 *  Simple macro that creates an AutoProfile instance.
	 *  'name' is the name of the sample section of code.
	 */
	#define PROFILE(name) AutoProfile profiler(name)
}

#endif // !PROFILER_H_