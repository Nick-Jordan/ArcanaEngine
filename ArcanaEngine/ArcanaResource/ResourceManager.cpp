#include "stdafx.h"
#include "ResourceManager.h"

#include "ArcanaLog.h"

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
	}
		
		
	void ResourceManager::initialize(const std::string& dataFile)
	{
		if(!_database.initialize(dataFile))
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
	
	void ResourceManager::addDefaultHeader(const std::string& filename)
	{
		addDefaultHeader(ResourceHeader(filename));
	}

	ResourceHeader ResourceManager::getDefaultHeader(const std::string& name)
	{
		if (_defaultHeaders.find(name) == _defaultHeaders.end())
		{
			LOGF(Error, ResourceLog, "Unable to find default header with name, \'%s\'", name.c_str());
			LOG(Info, ResourceLog, "Add the header as a default before trying to access it");

			return ResourceHeader();
		}

		return _defaultHeaders.at(name);
	}
	
	Resource* ResourceManager::loadResource(const std::string& name)
	{
		Resource resource = _database.getResource(name);
	
		if(resource.getName() == name)
		{
			std::map<std::string, createFunction>::iterator i = _resourceTypes.find(resource.getType());

			if (i != _resourceTypes.end())
			{
				Resource* creator = i->second(name, resource.getData());
				return creator;
			}
		}
		
		return nullptr;
	}
	
	Resource* ResourceManager::loadResource(const GlobalObjectID& id)
	{
		Resource resource = _database.getResource(id);
		
		if(resource.getId() == id)
		{
			std::map<std::string, createFunction>::iterator i = _resourceTypes.find(resource.getType());
			if (i != _resourceTypes.end()) 
			{
				Resource* creator = i->second(resource.getName(), resource.getData());
				return creator;
			}
		}
		
		return nullptr;
	}
	
	void ResourceManager::addType(const std::string& type, createFunction function)
	{
		_resourceTypes.emplace(type, function);
	}
	
	void ResourceManager::addDefaultHeader(const ResourceHeader& header)
	{
		_defaultHeaders.emplace(header.getType(), header);
	}
}