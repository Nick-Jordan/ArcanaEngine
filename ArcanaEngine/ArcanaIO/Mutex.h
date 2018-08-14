#ifndef MUTEX_H_
#define MUTEX_H_

#include "IODefines.h"

namespace Arcana
{
	class ARCANA_IO_API MutexContext;

	class ARCANA_IO_API Mutex
	{
	public:

		Mutex();

		~Mutex();

		void lock();

		void unlock();

	private:

		MutexContext* _mutexContext;
	};
}

#endif // !MUTEX_H_