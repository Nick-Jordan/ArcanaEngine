#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "ResourceDefines.h"

#include "ResourceDatabase.h"
#include "Mutex.h"
#include "Scheduler.h"

#include <future>
#include <queue>

namespace Arcana
{
	class ARCANA_RESOURCE_API ResourceManager;

	template<typename T>
	class LoadResourceTask : public Task
	{
		friend class ResourceManager;

	public:

		LoadResourceTask(T* r);

		LoadResourceTask(ResourceManager* manager);

		virtual void run() override;

		virtual void done() override;
		
		T* get() const;

	private:

		T* _resource;

		FindResourceTask* _findTask;

		ResourceManager* _manager;
	};
		
	class ARCANA_RESOURCE_API ResourceManager
	{
		friend class Resource;

		template<typename T>
		friend class LoadResourceTask;
		
	public:
	
		typedef Resource*(*createFunction) (const std::string& name, const std::string& type, const ResourceData& data);
	
	
		static ResourceManager& instance();
		
		ResourceManager();
		
		~ResourceManager();
		
		
		void initialize(ResourceDatabase* database);
		
		void finalize();
		
		const ResourceDatabase* getDatabase() const;
		
		bool reloadResources();
			
		template<typename T>
		LoadResourceTask<T>* loadResource(const GlobalObjectID& id);
		
		template<class T>
		T* findResource(const GlobalObjectID& id);

	private:
		
		void addType(const std::string& type, createFunction function);
		
		ResourceDatabase* _database;
		std::map<std::string, createFunction> _resourceTypes;	
		std::map<int64, Resource*> _resourceRegistry;
	};

	template<typename T>
	LoadResourceTask<T>* ResourceManager::loadResource(const GlobalObjectID& id)
	{
		T* r = findResource<T>(id);
		if (r)
		{
			return new LoadResourceTask<T>(r);
		}

		if (!_database)
			return nullptr;

		//LOGF(Info, CoreEngine, "create task");

		LoadResourceTask<T>* task = new LoadResourceTask<T>(this);
		//LOGF(Info, CoreEngine, "find task");
		FindResourceTask* findTask = _database->getResource(id);
		task->_findTask = findTask;
		//LOGF(Info, CoreEngine, "add dependency task");
		task->addDependency(findTask);

		//LOGF(Info, CoreEngine, "start task");
		_database->TaskScheduler->schedule(task);
		//LOGF(Info, CoreEngine, "end task");

		return task;
	}

	template<class T>
	T* ResourceManager::findResource(const GlobalObjectID& id)
	{
		std::map<int64, Resource*>::iterator iter = _resourceRegistry.find(id.getId());

		if (iter != _resourceRegistry.end())
		{
			iter->second->reference();
			return dynamic_cast<T*>(iter->second);
		}

		return nullptr;
	}

	template<typename T>
	LoadResourceTask<T>::LoadResourceTask(T* r) : _manager(nullptr), _resource(r)
	{

	}
	
	template<typename T>
	LoadResourceTask<T>::LoadResourceTask(ResourceManager* manager) : _manager(manager), _resource(nullptr)
	{

	}

	template<typename T>
	void LoadResourceTask<T>::run()
	{
		Resource* r = _findTask->getResource();

		if (!r || !_manager)
			return;

		std::map<std::string, ResourceManager::createFunction>::iterator i = _manager->_resourceTypes.find(r->getType());

		if (i != _manager->_resourceTypes.end())
		{
			Resource* creator = i->second(r->getName(), r->getType(), r->getData());
			creator->reference();

			_manager->_resourceRegistry.emplace(r->getId().getId(), creator);

			_resource = dynamic_cast<T*>(creator);
		}
	}

	template<typename T>
	void LoadResourceTask<T>::done()
	{
		
	}

	template<typename T>
	T* LoadResourceTask<T>::get() const
	{
		return _resource;
	}
}

#endif // !RESOURCE_MANAGER_H_