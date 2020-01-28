#include "Resource.h"

#include "ResourceManager.h"

namespace Arcana
{		
	Resource::Resource() : Object("Resource"), _id("null_resource"), _type("resource")
	{
	}
	
	Resource::Resource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
		: Object("Resource"), _id(id), _data(data), _type(type)
	{
	}
	
	Resource::Resource(const Resource& resource) : Object(resource),  _id(resource._id), _data(resource._data), _type(resource._type)
	{

	}

	Resource::~Resource()
	{
	}				
		
	const ResourceData& Resource::getData()const
	{
		return _data;
	}
	
	const GlobalObjectID& Resource::getId() const
	{
		return _id;
	}
		
	const std::string& Resource::getName() const
	{
		return _id.getName();
	}

	const std::string& Resource::getType() const
	{
		return _type;
	}
	
	bool Resource::operator==(const Resource& other)
	{
		return _id == other._id;
	}

	void Resource::addType(const std::string& type, Resource* (*createFunction) (const std::string& name, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler), bool needsContext)
	{
		ResourceManager::instance().addType(type, createFunction, needsContext);
	}
}