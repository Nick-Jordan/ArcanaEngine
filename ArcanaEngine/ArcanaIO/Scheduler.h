#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "IODefines.h"

#include "Task.h"
#include <vector>

namespace Arcana
{

	class ARCANA_IO_API Scheduler
	{
	public:

		static std::vector<Task*> SyncTaskList;


		Scheduler();//attributes

		~Scheduler();

		void schedule(Task* task);

	private:

		tf::Executor _executor;
	};

}

#endif // !SCHEDULER_H_