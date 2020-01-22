#ifndef TASK_H_
#define TASK_H_

#include "IODefines.h"

#include "Object.h"
#include "taskflow/taskflow.hpp"

namespace Arcana
{

	class ARCANA_IO_API Task : public Object
	{
		friend class Scheduler;

	public:

		Task();

		Task(const std::string& taskName);

		virtual ~Task();

		virtual void run() = 0;

		virtual void done() = 0;

		//virtual void syncDone() { };

		//virtual bool needsSyncDone() { return false; }

		void addDependency(Task* task);

		bool dependsOn(Task* task);

		bool isDependencyOf(Task* task);

		bool isDone() const;

		bool wasScheduled() const;

		void wait() const;

		//C is callable type that takes a (Task*) as an argument
		template<typename C>
		void runForAllDependencies(C&& callable);

		const std::string& getTaskName() const;

	private:

		void initialize(tf::Taskflow& taskflow);

		bool _done;
		bool _scheduled;
		std::string _taskName;

		tf::Task _task;
		tf::Taskflow _taskflow;

		std::vector<Task*> _dependencies;
	};

	template<typename C>
	inline void Task::runForAllDependencies(C&& callable)
	{
		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			(*i)->runForAllDependencies(callable);
		}

		callable(this);
	}
}

#endif // !TASK_H_