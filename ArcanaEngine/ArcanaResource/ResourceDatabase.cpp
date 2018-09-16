#include "stdafx.h"
#include "ResourceDatabase.h"
#include "XMLFile.h"
#include "ResourceManager.h"
#include "FileInputStream.h"

#include "ArcanaLog.h"

namespace Arcana
{
	ResourceDatabase::ResourceDatabase()
	{
	}
		
	ResourceDatabase::ResourceDatabase(const std::string& filename)
	{
		initialize(filename);
	}
		
	ResourceDatabase::~ResourceDatabase()
	{
	}
		
		
	bool ResourceDatabase::initialize(const std::string& filename)
	{
		XMLFile file(filename);

		//for (auto iter = file.getNodes().createConstIterator(); iter; ++iter)
		for (auto iter = file.getNodes().begin(); iter != file.getNodes().end(); ++iter)
		{
			XMLNode n = (*iter);
			if (n.getName() == "resource")
			{
				std::string name = "resource";
				bool defaultHeader = true;
				std::string header = "header";
				std::string data = "data";

				//for (auto iter = n.getChildren().createConstIterator(); iter; ++iter)
				for (auto iter = n.getChildren().begin(); iter != n.getChildren().end(); ++iter)
				{
					XMLNode c = (*iter);
					if (c.getName() == "name")
					{
						name = c.getValue();
					}
					else if (c.getName() == "header_default")
					{
						defaultHeader = c.getValue() == "true";
					}
					else if (c.getName() == "resource_header")
					{
						header = c.getValue();
					}
					else if (c.getName() == "resource_data")
					{
						data = c.getValue();
					}
				}

				ResourceHeader resourceHeader;
				if (defaultHeader)
				{
					resourceHeader = ResourceManager::instance().getDefaultHeader(header);
				}
				else
				{
					resourceHeader = ResourceHeader(header);
				}

				FileInputStream stream;
				if (!stream.open(data))
				{
					LOGF(Error, ResourceLog, "Error loading resource data file, \'%s\' for resource, \'%s\'", data.c_str(), name.c_str());
					return false;
				}

				_resources.add(Resource(name, ResourceData(Archive(stream), resourceHeader)));
			}
		}

		return true;
	}
	
	const Resource& ResourceDatabase::getResource(const std::string& name)
	{
		Resource* resource = _resources.findByPredicate([=](Resource resource) {return resource.getName() == name; });
		
		if(!resource)
		{
			LOGF(Error, ResourceLog, "Failed to find resource with name, \'%s\'", name.c_str());
			return Resource();
		}
		
		return *resource;
	}
		
	const Resource& ResourceDatabase::getResource(const GlobalObjectID& id)
	{
		Resource* resource = _resources.findByPredicate([=](Resource resource) {return resource.getId() == id; });	
		
		if(!resource)
		{
			LOGF(Error, ResourceLog, "Failed to find resource with name, \'%s\' and id, \'%d\'", id.getName().c_str(), id.getId());
			return Resource();
		}
		
		return *resource;
	}
}