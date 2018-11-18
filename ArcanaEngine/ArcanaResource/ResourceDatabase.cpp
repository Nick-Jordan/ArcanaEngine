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
		XMLFile file;

		if (!file.create(filename))
		{
			return false;
		}

		//for (auto iter = file.getNodes().createConstIterator(); iter; ++iter)
		for (auto iter = file.getNodes().begin(); iter != file.getNodes().end(); ++iter)
		{
			XMLNode n = (*iter);
			if (n.getName() == "resource")
			{
				std::string name = "resource";
				std::string type = "type";
				XMLNode dataNode;

				//for (auto iter = n.getChildren().createConstIterator(); iter; ++iter)
				for (auto iter = n.getChildren().begin(); iter != n.getChildren().end(); ++iter)
				{
					XMLNode c = (*iter);
					if (c.getName() == "name")
					{
						name = c.getValue();
					}
					else if (c.getName() == "type" || c.getName() == "resource_type")
					{
						type = c.getValue();
					}
					else if (c.getName() == "resource_data")
					{
						if (c.getChildren().size() > 0)
						{
							dataNode = c;
						}
						else
						{
							XMLFile dataFile(c.getValue());
							dataNode = dataFile.getRoot();
						}
					}
				}

				_resources.add(Resource(name, type, ResourceData(dataNode)));
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