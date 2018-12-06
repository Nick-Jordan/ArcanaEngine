#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "IODefines.h"

#include "Types.h"
#include "Thread.h"
#include "Task.h"
#include "ThreadSafeQueue.h"

#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>

namespace Arcana
{
	class ARCANA_IO_API Scheduler : public Object
	{
	private:

  		class ThreadWorker 
  		{
  		public:

		    ThreadWorker(Scheduler* scheduler, const int32 id);

		    void operator()();

	    private:

	    	int32 _id;
	    	Scheduler* _scheduler;
		};

	public:

		Scheduler(const int32 threads);

		~Scheduler();

		void initialize();

		void shutdown();

  		void schedule(Task* task);

  		bool isRunning() const;


  		Scheduler(const Scheduler&) = delete;
		Scheduler(Scheduler&&) = delete;

		Scheduler& operator=(const Scheduler&) = delete;
		Scheduler& operator=(Scheduler&&) = delete;

	private:

		bool _running;
  		ThreadSafeQueue<Task*> _queue;
		std::vector<std::unique_ptr<std::thread>> _threads;
		std::mutex _conditionalMutex;
		std::condition_variable _conditionalLock;
	};
}

#endif // !SCHEDULE_H_