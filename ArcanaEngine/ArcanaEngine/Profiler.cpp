#include "Profiler.h"

#include "Globals.h"

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

		if(GProfileManager)
		{
			GProfileManager->storeSample(_name, elapsedTime);
		}
	}
}