#include "Scheduler.h"

#include <iostream>

#include <thread>
#include <future>
#include <functional>
#include <memory>

namespace Arcana
{
	Scheduler::Scheduler() : Object("Scheduler"), _running(true)
	{
	}

	Scheduler::~Scheduler()
	{
	
	}

	void Scheduler::schedule(std::shared_ptr<Task> task)
	{		
		_threadPool.post(std::packaged_task<void()>(std::bind(&Task::run, task)));
	}

	bool Scheduler::isRunning() const
	{
		return _running;
	}
}