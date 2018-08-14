#ifndef WINDOWS_MUTEX_CONTEXT_H_
#define WINDOWS_MUTEX_CONTEXT_H_

#include "IODefines.h"

#include <Windows.h>

namespace Arcana
{
	class ARCANA_IO_API MutexContext
	{
	public:

		MutexContext();

		~MutexContext();

		void lock();

		void unlock();

	private:

		CRITICAL_SECTION _mutex;
	};

}

#endif // !WINDOWS_MUTEX_CONTEXT_H_