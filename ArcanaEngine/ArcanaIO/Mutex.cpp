#include "Mutex.h"

#include "MutexContext.h"
#include "CoreDefines.h"

namespace Arcana
{
	Mutex::Mutex()
	{
		_mutexContext = new MutexContext();
	}

	Mutex::~Mutex()
	{
		AE_DELETE(_mutexContext);
	}

	void Mutex::lock()
	{
		_mutexContext->lock();
	}

	void Mutex::unlock()
	{
		_mutexContext->unlock();
	}

}