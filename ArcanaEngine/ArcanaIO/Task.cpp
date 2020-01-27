#include "Task.h"

#include "ArcanaLog.h"
#include "Sleep.h"

namespace Arcana
{

	TaskBase::TaskBase() : Object("Task"), _taskName("Task"), _done(false), _scheduled(false)
	{
	}

	TaskBase::TaskBase(const std::string& taskName) : Object("Task"), _taskName(taskName), _done(false), _scheduled(false)
	{

	}

	TaskBase::~TaskBase()
	{
		//LOG(Info, CoreEngine, "releasing dependencies");

		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			//LOGF(Info, CoreEngine, "dependency: %p, %d", *i, (*i)->referenceCount());
			AE_RELEASE(*i);
		}
	}

	void TaskBase::addDependency(TaskBase* task)
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

	bool TaskBase::dependsOn(TaskBase* task)
	{
		bool result = false;

		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			TaskBase* t = *i;

			if (t == task)
			{
				result = true;
				break;
			}

			result |= t->dependsOn(task);
		}

		return result;
	}

	bool TaskBase::isDependencyOf(TaskBase* task)
	{
		return task->dependsOn(this);
	}

	bool TaskBase::isDone() const
	{
		return _done;
	}

	bool TaskBase::wasScheduled() const
	{
		return _scheduled;
	}

	void TaskBase::wait() const
	{
		if (wasScheduled())
		{
			while (!isDone())
			{
				AE_SLEEP(Time::fromMilliseconds(10));
			}
		}
	}

	const std::string& TaskBase::getTaskName() const
	{
		return _taskName;
	}

	void TaskBase::initialize(tf::Taskflow& taskflow)
	{
		//reference();

		_scheduled = true;

		_task = taskflow.emplace(std::bind(&TaskBase::run, this));

		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			//LOGF(Info, CoreEngine, "dependency: %p", *i);

			(*i)->initialize(taskflow);

			_task.gather((*i)->_task);
		}
	}

}