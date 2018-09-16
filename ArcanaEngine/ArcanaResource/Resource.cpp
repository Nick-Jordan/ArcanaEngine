#include "stdafx.h"
#include "Resource.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ResourceLog)
	
	
	Resource::Resource() : Object("Resource")
	{
	}
	
	Resource::Resource(const std::string& name, ResourceData& data)
		: Object("Resource"), _id(name), _data(data), _type(data.getHeader().getType())
	{
	}
	
	Resource::~Resource()
	{
	}				
		
	ResourceData& Resource::getData()
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