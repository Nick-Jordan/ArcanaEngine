#include "Scheduler.h"

#include <iostream>

namespace Arcana
{
	Scheduler::Scheduler(const int32 threads)
		: _threads(std::vector<std::unique_ptr<std::thread>>(threads)), _running(true)
	{
	}

	Scheduler::~Scheduler()
	{
	
	}

	void Scheduler::initialize()
	{
		for (int32 i = 0; i < _threads.size(); i++)
		{
			_threads[i] = std::unique_ptr<std::thread>(new std::thread(ThreadWorker(this, i)));
		}
	}

	void Scheduler::shutdown()
	{
		_running = false;
		_conditionalLock.notify_all();

		for (int32 i = 0; i < _threads.size(); i++)
		{
			if (_threads[i]->joinable())
				_threads[i]->join();
		}

		while (_queue.size() > 0)
		{
			Task* task = nullptr;
			_queue.dequeue(task);

			if (task)
			{
				AE_RELEASE(task);
			}
		}
	}

	void Scheduler::schedule(Task* task)
	{
		task->reference();
		_queue.enqueue(task);

		_conditionalLock.notify_one();
	}

	bool Scheduler::isRunning() const
	{
		return _running;
	}


	Scheduler::ThreadWorker::ThreadWorker(Scheduler* scheduler, const int32 id)
		: _scheduler(scheduler), _id(id)
	{
	}

	void Scheduler::ThreadWorker::operator()()
	{

		Task* task = nullptr;
		bool dequeued;
		while (_scheduler->isRunning())
		{
			{
				std::unique_lock<std::mutex> lock(_scheduler->_conditionalMutex);
				if (_scheduler->_queue.empty())
				{
					_scheduler->_conditionalLock.wait(lock);
				}
				dequeued = _scheduler->_queue.dequeue(task);
			}

			if (dequeued)
			{
				task->run();
				task->release();
			}
		}
	}
}