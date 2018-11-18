#include "Resource.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ResourceLog)
	
	
	Resource::Resource() : Object("Resource"), _id("null_resource"), _type("resource")
	{
	}
	
	Resource::Resource(const std::string& name, const std::string& type, const ResourceData& data)
		: Object("Resource"), _id(name), _data(data), _type(type)
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
}