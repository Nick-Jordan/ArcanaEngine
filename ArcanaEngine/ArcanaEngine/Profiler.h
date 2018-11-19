#ifndef PROFILER_H_
#define PROFILER_H_

#include "CoreDefines.h"

#include "Types.h"

namespace Arcana
{
	class ARCANA_CORE_API AutoProfile 
	{
	public:

		AutoProfile(const char* name);

		~AutoProfile();

	private:

		const char* _name;
		int64 _startTime;
	};
	
	#define PROFILE(name) AutoProfile profiler(name)
}

#endif // !PROFILER_H_