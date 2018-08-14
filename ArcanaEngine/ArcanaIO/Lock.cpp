#include "Lock.h"

#include "Mutex.h"

namespace Arcana
{
	Lock::Lock(Mutex& mutex) :
		_mutex(mutex)
	{
		_mutex.lock();
	}

	Lock::~Lock()
	{
		_mutex.unlock();
	}
}