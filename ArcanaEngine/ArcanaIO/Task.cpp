#include "Task.h"

namespace Arcana
{
	Task::Task(const std::string& type) : Object(type), _done(false)
	{

	}

	Task::~Task()
	{

	}

	bool Task::isDone() const
	{
		return _done;
	}
}