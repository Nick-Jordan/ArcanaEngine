#include "ResourceDatabase.h"
#include "XMLFile.h"
#include "ResourceManager.h"
#include "FileInputStream.h"

#include "ResourceLoggers.h"

namespace Arcana
{
	ResourceDatabase::ResourceDatabase() : Object("ResourceDatabase")
	{
		TaskScheduler = new Scheduler();
	}
		
	ResourceDatabase::ResourceDatabase(const std::string& type) : Object(type)
	{
	}
		
	ResourceDatabase::~ResourceDatabase()
	{
		/*if (!finalize())
		{
			LOG(Error, ResourceDatabaseLog, "Failed to finalize resource database upon destruction...");
			LOG(Info, ResourceDatabaseLog, "Connections and other data may not have been freed...");
		}*/

		AE_DELETE(TaskScheduler);
	}

	FindResourceTask::FindResourceTask() : Task("FindResourceTask")
	{

	}

	FindResourceTask::FindResourceTask(const std::string& taskName) : Task(taskName)
	{

	}

	FindResourceTask::~FindResourceTask()
	{

	}

	void FindResourceTask::run()
	{

	}

	void FindResourceTask::done()
	{

	}
}