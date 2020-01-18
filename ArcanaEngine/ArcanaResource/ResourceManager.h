#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "ResourceDefines.h"

#include "ResourceDatabase.h"
#include "Mutex.h"
#include "Scheduler.h"
#include "Callback.h"

#include <future>
#include <queue>

namespace Arcana
{
	class ARCANA_RESOURCE_API ResourceManager;

	template <typename T>
	using ResourceLoadedCallback = BaseCallback<void, T*>;

	template<typename T>
	class LoadResourceTask : public Task
	{
		friend class ResourceManager;

	public:

		LoadResourceTask(T* r);// , const ResourceLoadedCallback<T>& loadedCallback);

		LoadResourceTask(ResourceManager* manager);// , const ResourceLoadedCallback<T>& loadedCallback);

		virtual void run() override;

		virtual void done() override;
		
		T* get() const;

	private:

		Resource* _resource;
		T* _precreatedResource;

		bool _needsContext;

		FindResourceTask* _findTask;

		ResourceManager* _manager;

		ResourceLoadedCallback<T> _resourceLoadedCallback;
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
	
		typedef Resource*(*createFunction) (const std::string& name, const std::string& type, const ResourceData& data);
	
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
		LoadResourceTask<T>* loadResource(const GlobalObjectID& id);// , const ResourceLoadedCallback<T>& loadedCallback = ResourceLoadedCallback<T>());

		template<typename T>
		LoadResourceTask<T>* buildResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data);// , const ResourceLoadedCallback<T>& loadedCallback = ResourceLoadedCallback<T>());
		
		template<class T>
		T* findResource(const GlobalObjectID& id);

	private:
		
		void addType(const std::string& type, createFunction function, bool needsContext);
		
		ResourceDatabase* _database;
		std::map<std::string, CreatorStruct> _resourceTypes;
		std::map<int64, Resource*> _resourceRegistry;

	public://test
		std::vector<Task*> _resourcesTasks;
	};

	template<typename T>
	LoadResourceTask<T>* ResourceManager::loadResource(const GlobalObjectID& id)// , const ResourceLoadedCallback<T>& loadedCallback)
	{
		T* r = findResource<T>(id);
		if (r)
		{
			LoadResourceTask<T>* task = new LoadResourceTask<T>(r);// , loadedCallback);
			_resourcesTasks.push_back(task);
			return task;
		}

		if (!_database)
			return nullptr;

		//LOGF(Info, CoreEngine, "create task");

		LoadResourceTask<T>* task = new LoadResourceTask<T>(this);// , loadedCallback);

		_resourcesTasks.push_back(task);

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

	template<typename T>
	LoadResourceTask<T>* ResourceManager::buildResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)// , const ResourceLoadedCallback<T>& loadedCallback)
	{
		LoadResourceTask<T>* task = new LoadResourceTask<T>(this);// , loadedCallback);
		//LOGF(Info, CoreEngine, "find task");
		FindResourceTask* findTask = new BuildResourceTask(new Resource(id, type, data));
		task->_findTask = findTask;

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
	LoadResourceTask<T>::LoadResourceTask(T* r)//, const ResourceLoadedCallback<T>& loadedCallback) 
		: _manager(nullptr), _resource(nullptr), _precreatedResource(r), _needsContext(false)
	{

	}
	
	template<typename T>
	LoadResourceTask<T>::LoadResourceTask(ResourceManager* manager)//, const ResourceLoadedCallback<T>& loadedCallback)
		: _manager(manager), _resource(nullptr), _precreatedResource(nullptr), 
		_needsContext(false)
	{

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

			Resource* creator = i->second.CreateFunction(r->getName(), r->getType(), r->getData());
			creator->reference();

			_manager->_resourceRegistry.emplace(r->getId().getId(), creator);

			_resource = creator;
		}
	}

	template<typename T>
	void LoadResourceTask<T>::done()
	{
		/*if (_resource)
		{
			//need to syncinitialize...................
			_resourceLoadedCallback.executeIfBound(dynamic_cast<T*>(_resource));
		}
		else
		{
			_resourceLoadedCallback.executeIfBound(_precreatedResource);
		}*/
	}

	template<typename T>
	T* LoadResourceTask<T>::get() const
	{
		if (_resource)
		{
			if (_needsContext)
			{
				_resource->syncInitialize();
			}

			//remove from task list
			return dynamic_cast<T*>(_resource);
		}

		return _precreatedResource;
	}
}

#endif // !RESOURCE_MANAGER_H_