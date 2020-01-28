#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "ResourceDefines.h"

#include "ResourceData.h"
#include "GlobalObjectID.h"
#include "Object.h"
#include "ResourceLoggers.h"
#include "Scheduler.h"

//vld
#include <vld.h>

namespace Arcana
{	
	class ARCANA_RESOURCE_API Resource : public Object
	{
	public:
	
		template<class T, bool NeedsContext = false>
		class Type
		{
		public:
		
			static Resource* createFunction(const std::string& name, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			{
				return new T(name, type, data, dependencyScheduler);
			}

			Type(std::string name)
			{
				Resource::addType(name, createFunction, NeedsContext);
			}
		
		};
	
		Resource();
		
		Resource(const GlobalObjectID& id, const std::string& type, const ResourceData& data);

		Resource(const Resource& resource);
		
		virtual ~Resource();
		
		
		const ResourceData& getData() const;
		
		const GlobalObjectID& getId() const;
		
		const std::string& getName() const;

		const std::string& getType() const;
		
		virtual void syncInitialize() {};

		bool operator==(const Resource& other);

		static void addType(const std::string& type, Resource* (*createFunction) (const std::string& name, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler), bool needsContext);
		
	private:
	
		GlobalObjectID _id;
		
		ResourceData _data;

		std::string _type;
	};
	
}

#endif // !RESOURCE_H_