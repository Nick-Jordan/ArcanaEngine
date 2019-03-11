#include "ResourceManager.h"

#include "ArcanaLog.h"
#include "XMLFile.h"

namespace Arcana
{
	ResourceManager& ResourceManager::instance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		/*for (std::map<int64, Resource*>::iterator iter = _resourceRegistry.begin(); iter != _resourceRegistry.end(); iter++)
		{
			Resource* resource = iter->second;
			LOGF(Error, CoreEngine, "pointer: %p, %d", resource, resource->referenceCount());
			_resourceRegistry.erase(iter);
			AE_RELEASE(resource);
		}*/
	}


	void ResourceManager::initialize(const std::string& dataFile)
	{
		if (!_database.initialize(dataFile))
		{
			LOGF(Error, ResourceLog, "Error loading resource database from data file, \'%s\'", dataFile.c_str());
		}
	}

	void ResourceManager::finalize()
	{
	}

	const ResourceDatabase& ResourceManager::getDatabase() const
	{
		return _database;
	}

	bool ResourceManager::reloadResources()
	{
		return false;
	}

	Resource* ResourceManager::loadResource(const std::string& name)
	{
		Resource resource = _database.getResource(name);

		std::map<int64, Resource*>::iterator iter = _resourceRegistry.find(resource.getId().getId());

		if (iter != _resourceRegistry.end())
		{
			//iter->second->reference();
			return iter->second;
		}

		if (resource.getName() == name)
		{
			std::map<std::string, createFunction>::iterator i = _resourceTypes.find(resource.getType());

			if (i != _resourceTypes.end())
			{
				Resource* creator = i->second(name, resource.getType(), resource.getData());
				//creator->reference();

				//_resourceRegistry.emplace(resource.getId().getId(), creator);

				return creator;
			}
		}

		return nullptr;
	}

	Resource* ResourceManager::loadResource(const GlobalObjectID& id)
	{
		Resource resource = _database.getResource(id);

		std::map<int64, Resource*>::iterator iter = _resourceRegistry.find(id.getId());

		if (iter != _resourceRegistry.end())
		{
			//iter->second->reference();
			return iter->second;
		}

		if (resource.getId() == id)
		{
			std::map<std::string, createFunction>::iterator i = _resourceTypes.find(resource.getType());
			if (i != _resourceTypes.end())
			{
				Resource* creator = i->second(resource.getName(), resource.getType(), resource.getData());
				creator->reference();

				_resourceRegistry.emplace(id.getId(), creator);

				return creator;
			}
		}

		return nullptr;
	}

	Resource* ResourceManager::buildResource(const std::string& name, const std::string& type, const ResourceData& data)
	{
		std::map<std::string, createFunction>::iterator i = _resourceTypes.find(type);
		if (i != _resourceTypes.end())
		{
			Resource* creator = i->second(name, type, data);

			return creator;
		}

		return nullptr;
	}

	Resource* ResourceManager::loadResource(const std::string& file, const std::string& name)
	{
		XMLFile dataFile;

		if (!dataFile.create(file))
		{
			return nullptr;
		}
		
		XMLNode dataNode = dataFile.getRoot();

		std::string type;
		XMLNode n;

		bool breakLoop = false;
		for (auto iter = dataNode.getChildren().begin(); iter != dataNode.getChildren().end() && !breakLoop; ++iter)
		{
			n = (*iter);
			if (n.getName() == "resource")
			{
				for (auto iter = n.getChildren().begin(); iter != n.getChildren().end(); ++iter)
				{
					XMLNode c = (*iter);
					if (c.getName() == "name" && c.getValue() == name)
					{
						for (auto iter1 = n.getChildren().begin(); iter1 != n.getChildren().end(); ++iter1)
						{
							if ((*iter1).getName() == "resource_type")
							{
								type = (*iter1).getValue();
								breakLoop = true;
							}
						}
					}
				}
			}
		}

		if (!breakLoop)
		{
			return nullptr;
		}

		XMLNode c;
		for (auto iter = n.getChildren().begin(); iter != n.getChildren().end(); ++iter)
		{
			c = (*iter);
			if (c.getName() == "resource_data")
			{
				break;
			}
		}

		ResourceData data(c, false, file);

		std::map<std::string, createFunction>::iterator i = _resourceTypes.find(type);
		if (i != _resourceTypes.end())
		{
			Resource* creator = i->second(name, type, data);

			return creator;
		}

		return nullptr;
	}

	void ResourceManager::addType(const std::string& type, createFunction function)
	{
		_resourceTypes.emplace(type, function);
	}
}