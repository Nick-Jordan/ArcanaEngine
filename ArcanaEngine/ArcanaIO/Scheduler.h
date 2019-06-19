#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "IODefines.h"

#include "Task.h"

namespace Arcana
{

	class ARCANA_IO_API Scheduler
	{
	public:

		Scheduler();//attributes

		~Scheduler();

		void schedule(Task* task);

	private:

		tf::Executor _executor;
	};

}

#endif // !SCHEDULER_H_