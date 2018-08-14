#include "WindowsThreadContext.h"

#include "Thread.h"
#include "CoreDefines.h"
#include <process.h>

namespace Arcana
{

	ThreadContext::ThreadContext(Thread* parent)
	{
		_thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &ThreadContext::entryPoint, parent, 0, &_threadId));

		if (!_thread)
		{
			LOG(Error, ThreadLog, "Failed to create thread.");
		}
	}


	ThreadContext::~ThreadContext()
	{
		if (_thread)
		{
			CloseHandle(_thread);
		}
	}

	void ThreadContext::wait()
	{
		if (_thread)
		{
			AE_ASSERT(_threadId != GetCurrentThreadId());
			WaitForSingleObject(_thread, INFINITE);
		}
	}

	void ThreadContext::stop()
	{
		if (_thread)
		{
			TerminateThread(_thread, 0);
		}
	}

	unsigned int __stdcall ThreadContext::entryPoint(void* userData)
	{
		Thread* parent = static_cast<Thread*>(userData);

		parent->run();

		_endthreadex(0);

		return 0;
	}

}
