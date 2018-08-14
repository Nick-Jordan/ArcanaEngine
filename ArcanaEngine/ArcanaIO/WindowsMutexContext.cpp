#include "WindowsMutexContext.h"

namespace Arcana
{

	MutexContext::MutexContext()
	{
		InitializeCriticalSection(&_mutex);
	}

	MutexContext::~MutexContext()
	{
		DeleteCriticalSection(&_mutex);
	}

	void MutexContext::lock()
	{
		EnterCriticalSection(&_mutex);
	}

	void MutexContext::unlock()
	{
		LeaveCriticalSection(&_mutex);
	}

}
