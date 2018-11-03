#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "ResourceDefines.h"

#include "ResourceDatabase.h"

namespace Arcana
{
		
	class ARCANA_RESOURCE_API ResourceManager
	{
		friend class Resource;
		
	public:
	
		typedef Resource*(*createFunction) (const std::string& name, ResourceData& data);
	
	
		static ResourceManager& instance();
		
		ResourceManager();
		
		~ResourceManager();
		
		
		void initialize(const std::string& dataFile);
		
		void finalize();
		
		const ResourceDatabase& getDatabase() const;
		
		bool reloadResources();
		
		void addDefaultHeader(const std::string& filename);

		ResourceHeader getDefaultHeader(const std::string& name);
			
		
		template<typename T>
		T loadResource(const std::string& name);
		
		template<typename T>
		T loadResource(const GlobalObjectID& id);
		
	private:
	
		Resource* loadResource(const std::string& name);
		
		Resource* loadResource(const GlobalObjectID& id);
		
		void addType(const std::string& type, createFunction function);
	
		void addDefaultHeader(const ResourceHeader& header);
		
		ResourceDatabase _database;
		std::map<std::string, createFunction> _resourceTypes;	
		std::map<std::string, ResourceHeader> _defaultHeaders;
	};
	
	
	template<typename T>
	inline T ResourceManager::loadResource(const std::string& name)
	{
		Resource* resource = loadResource(name);
		
		if(resource != nullptr)
		{
			T* t = dynamic_cast<T*>(resource);
			T returnObject = *t;
			resource->release();
			return returnObject;
		}
		
		resource->release();

		LOGF(Error, ResourceLog, "Failed to load resource with name, \'%s\'", name.c_str());
		return T();
	}
	
	template<typename T>
	inline T ResourceManager::loadResource(const GlobalObjectID& id)
	{
		Resource* resource = loadResource(id);
		
		if(resource != nullptr)
		{
			T* t = dynamic_cast<T*>(resource);
			T returnObject = *t;
			resource->release();
			return returnObject;
		}
		
		resource->release();

		LOGF(Error, ResourceLog, "Failed to load resource with name, \'%s\' and id, \'%d\'", id.getName().c_str(), id.getId());
		return T();
	}
}

#endif // !RESOURCE_MANAGER_H_