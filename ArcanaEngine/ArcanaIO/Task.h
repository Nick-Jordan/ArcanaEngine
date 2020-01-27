#ifndef TASK_H_
#define TASK_H_

#include "IODefines.h"

#include "Object.h"
#include "taskflow/taskflow.hpp"

namespace Arcana
{

	class ARCANA_IO_API TaskBase : public Object
	{
		friend class Scheduler;

	public:

		TaskBase();

		TaskBase(const std::string& taskName);

		virtual ~TaskBase();

		virtual void run() = 0;

		virtual void done() = 0;

		void addDependency(TaskBase* task);

		bool dependsOn(TaskBase* task);

		bool isDependencyOf(TaskBase* task);

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

		std::vector<TaskBase*> _dependencies;
	};

	template<typename TaskResult>
	class Task : public TaskBase
	{
	public:

		Task() : TaskBase("BasicTask"), Result(nullptr) {};

		Task(const std::string& taskName) : TaskBase("BasicTask"), Result(nullptr) {};

		virtual ~Task() {};

		//virtual void syncDone() { };

		//virtual bool needsSyncDone() { return false; }

		TaskResult* get() const
		{
			return Result;
		};

	protected:

		TaskResult* Result;
	};

	typedef Task<void> BasicTask;

	template<typename C>
	inline void TaskBase::runForAllDependencies(C&& callable)
	{
		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			(*i)->runForAllDependencies(callable);
		}

		callable(this);
	}
}

#endif // !TASK_H_