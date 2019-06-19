#include "Task.h"

#include "ArcanaLog.h"
#include "Sleep.h"

namespace Arcana
{

	Task::Task() : Object("Task"), _taskName("Task"), _done(false), _scheduled(false)
	{
	}

	Task::Task(const std::string& taskName) : Object("Task"), _taskName(taskName), _done(false), _scheduled(false)
	{

	}

	Task::~Task()
	{
		//LOG(Info, CoreEngine, "releasing dependencies");

		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			//LOGF(Info, CoreEngine, "dependency: %p, %d", *i, (*i)->referenceCount());
			AE_RELEASE(*i);
		}
	}

	void Task::addDependency(Task* task)
	{
		if (!task)
		{
			return;
		}

		task->reference();

		if (!isDependencyOf(task))
		{
			_dependencies.push_back(task);
		}
	}

	bool Task::dependsOn(Task* task)
	{
		bool result = false;

		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			Task* t = *i;

			if (t == task)
			{
				result = true;
				break;
			}

			result |= t->dependsOn(task);
		}

		return result;
	}

	bool Task::isDependencyOf(Task* task)
	{
		return task->dependsOn(this);
	}

	bool Task::isDone() const
	{
		return _done;
	}

	bool Task::wasScheduled() const
	{
		return _scheduled;
	}

	void Task::wait() const
	{
		if (wasScheduled())
		{
			while (!isDone())
			{
				AE_SLEEP(Time::fromMilliseconds(10));
			}
		}
	}

	const std::string& Task::getTaskName() const
	{
		return _taskName;
	}

	void Task::initialize(tf::Taskflow& taskflow)
	{
		//reference();

		_scheduled = true;

		_task = taskflow.emplace(std::bind(&Task::run, this));

		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			//LOGF(Info, CoreEngine, "dependency: %p", *i);

			(*i)->initialize(taskflow);

			_task.gather((*i)->_task);
		}
	}

}