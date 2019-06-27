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

	ResourceManager::ResourceManager() : _database(nullptr)
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

	void ResourceManager::addType(const std::string& type, createFunction function)
	{
		_resourceTypes.emplace(type, function);
	}

	BuildResourceTask::BuildResourceTask(Resource* resource) 
		: FindResourceTask("BuildResourceTask")
	{
		_resource = resource;
	}
}