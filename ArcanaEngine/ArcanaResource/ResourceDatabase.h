#ifndef RESOURCE_DATABASE_H_
#define RESOURCE_DATABASE_H_

#include "ResourceDefines.h"

#include "Types.h"
#include "Resource.h"
#include "Object.h"

#include "Scheduler.h"

namespace Arcana
{
		//??DATABASE ID???

	class ARCANA_RESOURCE_API FindResourceTask : public Task
	{
		friend class ResourceManager;

	public:

		FindResourceTask();

		FindResourceTask(const std::string& taskName);

		~FindResourceTask();

		virtual void run() override;

		virtual void done() override;

		Resource* getResource() const;

	protected:

		Resource* _resource;
	};
	
	class ARCANA_RESOURCE_API ResourceDatabase : public Object
	{
	public:
			
		ResourceDatabase();
		
		ResourceDatabase(const std::string& type);
		
		virtual ~ResourceDatabase();
		
		virtual bool initialize() = 0;

		virtual bool finalize() = 0;

		virtual FindResourceTask* getResource(const GlobalObjectID& id) = 0;

		virtual Resource* findResource(const GlobalObjectID& id) = 0;

	public:

		Scheduler* TaskScheduler;
	};
	
}

#endif // !RESOURCE_DATABASE_H_