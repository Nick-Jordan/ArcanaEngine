#ifndef LOCK_H_
#define LOCK_H_

#include "IODefines.h"

namespace Arcana
{
	class ARCANA_IO_API Mutex;

	class ARCANA_IO_API Lock
	{
	public:

		explicit Lock(Mutex& mutex);

		~Lock();

	private:

		Mutex& _mutex;
	};

}


#endif // !LOCK_H_