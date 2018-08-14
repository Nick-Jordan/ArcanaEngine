#ifndef WINDOWS_THREAD_CONTEXT_H_
#define WINDOWS_THREAD_CONTEXT_H_

#include "IODefines.h"

#include <Windows.h>

namespace Arcana
{
	class Thread;

	class ARCANA_IO_API ThreadContext
	{
	public:

		ThreadContext(Thread* parent);

		~ThreadContext();

		void wait();

		void stop();

	private:

		static unsigned int __stdcall entryPoint(void* userData);

		HANDLE _thread;
		unsigned int _threadId;
	};

}
#endif // !WINDOWS_THREAD_CONTEXT_H_

