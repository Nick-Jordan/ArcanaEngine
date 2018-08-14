#include "Thread.h"

#include "ThreadContext.h"
#include "CoreDefines.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ThreadLog)

	Thread::~Thread()
	{
		wait();

		AE_DELETE(_function);
	}

	void Thread::start()
	{
		wait();
		_context = new ThreadContext(this);
	}

	void Thread::wait()
	{
		if (_context)
		{
			_context->wait();
			AE_DELETE(_context);
		}
	}

	void Thread::stop()
	{
		if (_context)
		{
			_context->stop();
			AE_DELETE(_context);
		}
	}

	void Thread::run()
	{
		_function->run();
	}
}