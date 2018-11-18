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
	
		typedef Resource*(*createFunction) (const std::string& name, const std::string& type, const ResourceData& data);
	
	
		static ResourceManager& instance();
		
		ResourceManager();
		
		~ResourceManager();
		
		
		void initialize(const std::string& dataFile);
		
		void finalize();
		
		const ResourceDatabase& getDatabase() const;
		
		bool reloadResources();
			
		
		template<typename T>
		T* loadResource(const std::string& name);
		
		template<typename T>
		T* loadResource(const GlobalObjectID& id);

		template<typename T>
		T* buildResource(const std::string& name, const std::string& type, const ResourceData& data);
		
	private:
	
		Resource* loadResource(const std::string& name);
		
		Resource* loadResource(const GlobalObjectID& id);

		Resource* buildResource(const std::string& name, const std::string& type, const ResourceData& data);
		
		void addType(const std::string& type, createFunction function);
		
		ResourceDatabase _database;
		std::map<std::string, createFunction> _resourceTypes;	
		std::map<int64, Resource*> _resourceRegistry;
	};
	
	
	template<typename T>
	inline T* ResourceManager::loadResource(const std::string& name)
	{
		Resource* resource = loadResource(name);
		
		if(resource != nullptr)
		{
			T* t = dynamic_cast<T*>(resource);
			if (t)
			{
				t->reference();
				return t;
			}
		}
		
		LOGF(Error, ResourceLog, "Failed to load resource with name, \'%s\'", name.c_str());
		return nullptr;
	}
	
	template<typename T>
	inline T* ResourceManager::loadResource(const GlobalObjectID& id)
	{
		Resource* resource = loadResource(id);
		
		if(resource != nullptr)
		{
			T* t = dynamic_cast<T*>(resource);
			if (t)
			{
				t->reference();
				return t;
			}
		}
		
		LOGF(Error, ResourceLog, "Failed to load resource with name, \'%s\' and id, \'%d\'", id.getName().c_str(), id.getId());
		return nullptr;
	}

	template<typename T>
	inline T* ResourceManager::buildResource(const std::string& name, const std::string& type, const ResourceData& data)
	{
		Resource* resource = buildResource(name, type, data);

		if (resource != nullptr)
		{
			T* t = dynamic_cast<T*>(resource);
			if (t)
			{
				t->reference();
				return t;
			}
		}

		LOGF(Error, ResourceLog, "Failed to build resource with type, \'%s\'", type.c_str());
		return nullptr;
	}
}

#endif // !RESOURCE_MANAGER_H_