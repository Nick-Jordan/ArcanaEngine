#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "ResourceDefines.h"

#include "ResourceDatabase.h"
#include "Mutex.h"
#include "Callback.h"
#include "Lock.h"

#include "UUID.h"

#include <future>
#include <queue>

namespace Arcana
{
	class ARCANA_RESOURCE_API ResourceManager;

	template <typename T>
	using ResourceLoadedCallback = BaseCallback<void, T*>;

	class LoadResourceTaskBase : public Task
	{
	public:

		LoadResourceTaskBase(const std::string& name) : Task(name), TaskReady(false){}

		virtual void finalize() = 0;

		virtual void runCallback() = 0;

		std::atomic<bool> TaskReady;
	};

	template<typename T>
	class LoadResourceTask : public LoadResourceTaskBase
	{
		friend class ResourceManager;

	public:

		LoadResourceTask(T* r, const ResourceLoadedCallback<T>& loadedCallback);

		LoadResourceTask(ResourceManager* manager, const ResourceLoadedCallback<T>& loadedCallback);

		~LoadResourceTask();

		virtual void run() override;

		virtual void done() override;

		virtual void finalize() override;

		virtual void runCallback() override;

		//virtual bool needsSyncDone() override;

		//virtual void syncDone() override;
		
		T* get();

	private:

		Resource* _resource;
		T* _precreatedResource;

		bool _needsContext;

		FindResourceTask* _findTask;

		ResourceManager* _manager;

		ResourceLoadedCallback<T> _resourceLoadedCallback;

		Scheduler* _dependecyScheduler;
	};

	class ARCANA_RESOURCE_API BuildResourceTask : public FindResourceTask
	{
	public: 

		BuildResourceTask(Resource* resource);
	};
		
	class ARCANA_RESOURCE_API ResourceManager
	{
		friend class Resource;

		template<typename T>
		friend class LoadResourceTask;
		
	public:
	
		typedef Resource*(*createFunction) (const std::string& name, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler);
	
		struct CreatorStruct
		{
			createFunction CreateFunction;
			bool NeedsContext;
		};

		static ResourceManager& instance();
		
		ResourceManager();
		
		~ResourceManager();
		
		
		void initialize(ResourceDatabase* database);
		
		void finalize();
		
		const ResourceDatabase* getDatabase() const;
		
		bool reloadResources();

		template<typename T>
		LoadResourceTask<T>* loadResource(const GlobalObjectID& id, Scheduler* scheduler = nullptr);

		template<typename T>
		LoadResourceTask<T>* loadResource(const GlobalObjectID& id, const ResourceLoadedCallback<T>& loadedCallback, Scheduler* scheduler = nullptr);

		template<typename T>
		LoadResourceTask<T>* buildResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* scheduler = nullptr);

		template<typename T>
		LoadResourceTask<T>* buildResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, const ResourceLoadedCallback<T>& loadedCallback, Scheduler* scheduler = nullptr);
		
		template<class T>
		T* findResource(const GlobalObjectID& id);

		void checkPendingResources();

		void finalizePendingResources();

	private:
		
		void addType(const std::string& type, createFunction function, bool needsContext);
		
		ResourceDatabase* _database;
		std::map<std::string, CreatorStruct> _resourceTypes;
		std::map<UUID, Resource*> _resourceRegistry;

		std::vector<LoadResourceTaskBase*> _pendingResourceTasks;

		Mutex _registryMutex;
	};

	template<typename T>
	LoadResourceTask<T>* ResourceManager::loadResource(const GlobalObjectID& id, Scheduler* scheduler)
	{
		ResourceLoadedCallback<T> callback;
		return loadResource(id, callback, scheduler);
	}

	template<typename T>
	LoadResourceTask<T>* ResourceManager::loadResource(const GlobalObjectID& id, const ResourceLoadedCallback<T>& loadedCallback, Scheduler* scheduler)
	{
		if (id.getId().isEmpty())
			return nullptr;

		//first check if resource has already been loaded
		T* r = findResource<T>(id);
		if (r)
		{
			LoadResourceTask<T>* task = new LoadResourceTask<T>(r, loadedCallback);
			return task;
		}

		if (!_database)
			return nullptr;

		//LOGF(Info, CoreEngine, "create task");

		LoadResourceTask<T>* task = new LoadResourceTask<T>(this, loadedCallback);

		//LOGF(Info, CoreEngine, "find task");
		FindResourceTask* findTask = _database->getResource(id);
		task->_findTask = findTask;
		//LOGF(Info, CoreEngine, "add dependency task");
		task->addDependency(findTask);

		//LOGF(Info, CoreEngine, "start task");
		if (scheduler)
		{
			scheduler->schedule(task);
		}
		else
		{
			_database->TaskScheduler->schedule(task);
		}

		//probably need mutex
		_pendingResourceTasks.push_back(task);
		//LOGF(Info, CoreEngine, "end task");

		return task;
	}


	template<typename T>
	LoadResourceTask<T>* ResourceManager::buildResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* scheduler)
	{
		ResourceLoadedCallback<T> callback;
		return buildResource(id, type, data, callback, scheduler);
	}

	template<typename T>
	LoadResourceTask<T>* ResourceManager::buildResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, const ResourceLoadedCallback<T>& loadedCallback, Scheduler* scheduler)
	{
		LoadResourceTask<T>* task = new LoadResourceTask<T>(this, loadedCallback);
		//LOGF(Info, CoreEngine, "find task");
		FindResourceTask* findTask = new BuildResourceTask(new Resource(id, type, data));
		task->_findTask = findTask;

		//LOGF(Info, CoreEngine, "start task");
		if (scheduler)
		{
			scheduler->schedule(task);
		}
		else
		{
			_database->TaskScheduler->schedule(task);
		}
		//LOGF(Info, CoreEngine, "end task");

		//probably need mutex
		_pendingResourceTasks.push_back(task);

		return task;
	}

	template<class T>
	T* ResourceManager::findResource(const GlobalObjectID& id)
	{
		Lock lock(_registryMutex);

		std::map<UUID, Resource*>::iterator iter = _resourceRegistry.find(id.getId());

		if (iter != _resourceRegistry.end())
		{
			iter->second->reference();
			return dynamic_cast<T*>(iter->second);
		}

		return nullptr;
	}

	template<typename T>
	LoadResourceTask<T>::LoadResourceTask(T* r, const ResourceLoadedCallback<T>& loadedCallback)
		: LoadResourceTaskBase("LoadResourceRask"), _manager(nullptr), _resource(nullptr), _precreatedResource(r), _needsContext(false),
		_resourceLoadedCallback(loadedCallback)
	{
		_resourceLoadedCallback.executeIfBound(_precreatedResource);
	}
	
	template<typename T>
	LoadResourceTask<T>::LoadResourceTask(ResourceManager* manager, const ResourceLoadedCallback<T>& loadedCallback)
		: LoadResourceTaskBase("LoadResourceRask"), _manager(manager), _resource(nullptr), _precreatedResource(nullptr),
		_needsContext(false), _resourceLoadedCallback(loadedCallback)
	{
		_dependecyScheduler = new Scheduler();
	}

	template<typename T>
	LoadResourceTask<T>::~LoadResourceTask()
	{
		AE_DELETE(_dependecyScheduler);
	}

	template<typename T>
	void LoadResourceTask<T>::run()
	{
		Resource* r = _findTask->getResource();

		if (!r || !_manager)
			return;

		std::map<std::string, ResourceManager::CreatorStruct>::iterator i = _manager->_resourceTypes.find(r->getType());

		if (i != _manager->_resourceTypes.end())
		{
			_needsContext = i->second.NeedsContext;

			Resource* creator = i->second.CreateFunction(r->getName(), r->getType(), r->getData(), _dependecyScheduler);
			creator->reference();

			Lock lock(_manager->_registryMutex);
			_manager->_resourceRegistry.emplace(r->getId().getId(), creator);

			_resource = creator;
		}
	}

	template<typename T>
	void LoadResourceTask<T>::done()
	{
		/*if (!_needsContext)
		{
			if (_resource)
			{
				_resourceLoadedCallback.executeIfBound(dynamic_cast<T*>(_resource));
			}
		}*/
	}

	template<typename T>
	void LoadResourceTask<T>::finalize()
	{
		if (_resource)
		{
			if (_needsContext)
			{
				_resource->syncInitialize();
			}

			TaskReady = true;
		}
	}

	template<typename T>
	void LoadResourceTask<T>::runCallback()
	{
		if (_resource)
		{
			_resourceLoadedCallback.executeIfBound(dynamic_cast<T*>(_resource));
		}
	}

	/*template<typename T>
	bool LoadResourceTask<T>::needsSyncDone()
	{
		return true;
	}

	template<typename T>
	void LoadResourceTask<T>::syncDone()
	{
		if (_needsContext)
		{
			if (_resource)
			{
				_resource->syncInitialize();
				_resourceLoadedCallback.executeIfBound(dynamic_cast<T*>(_resource));
			}
		}
	}*/

	template<typename T>
	T* LoadResourceTask<T>::get()
	{
		if (_resource)
		{
			if (_needsContext)
			{
				_resource->syncInitialize();
			}

			//Scheduler::SyncTaskList.erase(std::remove(Scheduler::SyncTaskList.begin(), Scheduler::SyncTaskList.end(), this), Scheduler::SyncTaskList.end());

			Lock lock(ResourceManager::instance()._registryMutex);
			ResourceManager::instance()._pendingResourceTasks.erase(
				std::remove(
					ResourceManager::instance()._pendingResourceTasks.begin(),
					ResourceManager::instance()._pendingResourceTasks.end(),
					this),
				ResourceManager::instance()._pendingResourceTasks.end());

			T* finalResource = dynamic_cast<T*>(_resource);
			_resourceLoadedCallback.executeIfBound(finalResource);
			return finalResource;
		}

		return _precreatedResource;
	}
}

#endif // !RESOURCE_MANAGER_H_