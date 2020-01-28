#include "ResourceManager.h"

#include "ArcanaLog.h"
#include "XMLFile.h"
#include "Lock.h"

namespace Arcana
{
	ResourceManager& ResourceManager::instance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager::ResourceManager() : _database(nullptr), _numPendingResourceTasks(0)
	{
	}

	ResourceManager::~ResourceManager()
	{
		finalize();

		/*for (std::map<int64, Resource*>::iterator iter = _resourceRegistry.begin(); iter != _resourceRegistry.end(); iter++)
		{
			Resource* resource = iter->second;
			LOGF(Error, CoreEngine, "pointer: %p, %d", resource, resource->referenceCount());
			_resourceRegistry.erase(iter);
			AE_RELEASE(resource);
		}*/
	}


	void ResourceManager::initialize(ResourceDatabase* database)
	{
		_database = database;

		if (_database)
		{
			_database->reference();
		}
		else
		{
			LOG(Error, ResourceLog, "Error initializing resource manager: resource database was null");
		}
	}

	void ResourceManager::finalize()
	{
		AE_RELEASE(_database);
	}

	const ResourceDatabase* ResourceManager::getDatabase() const
	{
		return _database;
	}

	bool ResourceManager::reloadResources()
	{
		return false;
	}
	
		//Check registry
			//return if found (nothing else to do)

		//Check for database
			//return if null

		// 1) Get resourceData task (from database)

		//When 1) is finished
			// 2) Build resource task

		//When 2) is finished, we have our resource

	LoadResourceTaskBase* ResourceManager::findResource(const GlobalObjectID& id)
	{
		Lock lock(_registryMutex);

		std::map<UUID, LoadResourceTaskBase*>::iterator iter = _resourceRegistry.find(id.getId());

		if (iter != _resourceRegistry.end())
		{
			//iter->second->reference();
			return iter->second;
		}

		return nullptr;
	}

	void ResourceManager::checkPendingResources()
	{
		if (_numPendingResourceTasks <= 0)
		{
			return;
		}

		Lock lock(_registryMutex);

		const int32 ResourceLimit = 5;

		uint32 count = 0;
		for (auto i = _pendingResourceTasks.begin(); i != _pendingResourceTasks.end();)
		{
			if (count < ResourceLimit)
			{
				LoadResourceTaskBase* task = *i;

				if (!task)
				{
					i = _pendingResourceTasks.erase(i);
					_numPendingResourceTasks = _numPendingResourceTasks - 1;
					count++;
					continue;
				}

				if (!task->TaskReady)
				{
					if (task->isDone())
					{
						task->finalize();
					}
					i++;
					count++;
				}
			}
			else
			{
				return;
			}
		}
	}

	void ResourceManager::finalizePendingResources()
	{
		if (_numPendingResourceTasks <= 0)
		{
			return;
		}

		Lock lock(_registryMutex);

		const int32 ResourceLimit = 5;

		uint32 count = 0;
		for (auto i = _pendingResourceTasks.begin(); i != _pendingResourceTasks.end();)
		{
			if (count < ResourceLimit)
			{
				LoadResourceTaskBase* task = *i;

				if (!task)
				{
					i = _pendingResourceTasks.erase(i);
					_numPendingResourceTasks = _numPendingResourceTasks - 1;
					count++;
					continue;
				}

				if (task->TaskReady)
				{
					task->runCallback();

					i = _pendingResourceTasks.erase(i);
					_numPendingResourceTasks = _numPendingResourceTasks - 1;
				}
				else
				{
					i++;
				}
			}
			else
			{
				return;
			}
			count++;
		}
	}

	void ResourceManager::addType(const std::string& type, createFunction function, bool needsContext)
	{
		CreatorStruct creatorStruct;
		creatorStruct.CreateFunction = function;
		creatorStruct.NeedsContext = needsContext;

		_resourceTypes.emplace(type, creatorStruct);
	}

	BuildResourceTask::BuildResourceTask(Resource* resource) 
		: FindResourceTask("BuildResourceTask")
	{
		Result = resource;
	}
}