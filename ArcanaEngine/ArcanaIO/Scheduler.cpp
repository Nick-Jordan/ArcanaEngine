#include "Scheduler.h"

#include "ArcanaLog.h"

namespace Arcana
{
	//std::vector<Task*> Scheduler::SyncTaskList;

	Scheduler::Scheduler()
	{
	}

	Scheduler::~Scheduler()
	{
	}

	void Scheduler::schedule(Task* task)
	{
		if (!task)
		{
			return;
		}

		/*if (task->needsSyncDone())
		{
			SyncTaskList.push_back(task);
		}*/

		task->initialize(task->_taskflow);

		//LOGF(Info, CoreEngine, "task: %p, %d", task, task->referenceCount());

		_executor.run(task->_taskflow, [task]() {

			//LOGF(Info, CoreEngine, "task: %p", task);

			task->runForAllDependencies([](Task* t) {

				if (t)
				{
					t->done();
					t->_done = true;
				}
			});

			//task->release();
		});
	}
}