#include "Profiler.h"

#include "TimerContext.h"
#include "ProfileManager.h"

namespace Arcana
{

	AutoProfile::AutoProfile(const char* name) : _name(name)
	{
		_startTime = TimerContext::getCurrentTime();
	}

	AutoProfile::~AutoProfile()
	{
		int64 endTime = TimerContext::getCurrentTime();
		int64 elapsedTime = endTime - _startTime;

		ProfileManager::instance().storeSample(_name, elapsedTime);
	}
}