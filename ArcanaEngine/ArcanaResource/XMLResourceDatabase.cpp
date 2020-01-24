#include "XMLResourceDatabase.h"

#include "ResourceManager.h"
#include "XMLFile.h"
#include "StringUtils.h"

namespace Arcana
{

	XMLResourceDatabase::XMLResourceDatabase() : ResourceDatabase()
	{
	}

	XMLResourceDatabase::~XMLResourceDatabase()
	{
	}

	//temp
	bool isXMLFilePath(const std::string& path)
	{
		std::vector<std::string> v = StringUtils::split(path, ".");
		return v.size() >= 2 && v.back() == "xml";
	}

	bool XMLResourceDatabase::initialize()
	{
		XMLFile file;

		if (!file.create(_filename))
		{
			return false;
		}

		uint32 index = 0;

		for (auto iter = file.getNodes().begin(); iter != file.getNodes().end(); ++iter)
		{
			XMLNode n = (*iter);
			if (n.getName() == "resource")
			{
				XMLNode* nameNode = n.findChild("name");
				XMLAttribute* typeAttr = n.findAttribute("type");

				GlobalObjectID guid(nameNode ? nameNode->getValue() : "resource_" + std::to_string(index));
				std::string type = typeAttr ? typeAttr->getValue() : "resource";

				//temp log
				//LOGF(Info, ResourceDatabaseLog, "resource[ type: %s, name: %s, guid: %s ]", type.c_str(), guid.getName().c_str(), guid.getIdString().c_str());

				XMLNode* dataNode = n.findChild("resource_data");

				if (!dataNode)
				{
					LOGF(Warning, ResourceDatabaseLog, "Failed to create resource: the node did not contain a 'resource_data' child.");
					continue;
				}

				XMLNode resourceDataNode;

				std::string path = dataNode->getValue();
				if (isXMLFilePath(path))
				{
					XMLFile dataFile;

					if (!dataFile.create(path))
					{
						LOGF(Warning, ResourceDatabaseLog, "Failed to create resource: unable to load specified resource data file, \'%s\'", path.c_str());
						continue;
					}

					if (dataFile.getRoot().getName() != "resource_data")
					{
						LOGF(Warning, ResourceDatabaseLog, "Failed to create resource: resource data XML file root node named \'%s\' instead of 'resource_data'", dataFile.getRoot().getName().c_str());
						continue;
					}

					resourceDataNode = dataFile.getRoot();
				}
				else
				{
					resourceDataNode = *dataNode;
				}

				_resources.add(Resource(guid, type, ResourceData(resourceDataNode)));
			}

			index++;
		}

		return true;
	}

	bool XMLResourceDatabase::finalize()
	{

		return false;
	}

	FindResourceTask* XMLResourceDatabase::getResource(const GlobalObjectID& id)
	{
		FindXMLResourceTask* task =	new FindXMLResourceTask(_resources, id);

		return task;
	}

	Resource* XMLResourceDatabase::findResource(const GlobalObjectID& id)
	{
		return _resources.findByPredicate([=](Resource resource) {return resource.getId() == id; });
	}

	XMLResourceDatabase* XMLResourceDatabase::create(const std::string& filename)
	{
		XMLResourceDatabase* database = new XMLResourceDatabase();

		database->_filename = filename;

		if (!database->initialize())
		{
			AE_DELETE(database);
			return nullptr;
		}

		return database;
	}


	FindXMLResourceTask::FindXMLResourceTask(Array<Resource>& array, const GlobalObjectID& id) 
		: FindResourceTask("FindXMLResourceTask"), _array(array), _id(id)
	{

	}

	void FindXMLResourceTask::run()
	{
		//LOG(Info, CoreEngine, "running task");
		Resource* r = _array.findByPredicate([=](Resource resource) {return resource.getId() == _id; });

		//LOGF(Info, CoreEngine, "resource?: %p", r);
		_resource = r;

		//LOG(Info, CoreEngine, "task done");
	}

	void FindXMLResourceTask::done()
	{

	}
}
