#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "IODefines.h"

#include "Types.h"
#include "Task.h"
#include "../Dependencies/include/thread_pool/thread_pool.hpp"

namespace Arcana
{
	class ARCANA_IO_API Scheduler : public Object
	{
	public:

		Scheduler();

		~Scheduler();

  		void schedule(std::shared_ptr<Task> task);

  		bool isRunning() const;

  		Scheduler(const Scheduler&) = delete;
		Scheduler(Scheduler&&) = delete;

		Scheduler& operator=(const Scheduler&) = delete;
		Scheduler& operator=(Scheduler&&) = delete;

	private:

		bool _running;
		tp::ThreadPool _threadPool;
	};
}

#endif // !SCHEDULE_H_