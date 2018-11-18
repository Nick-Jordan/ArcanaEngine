#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "ResourceDefines.h"

#include "ResourceData.h"
#include "GlobalObjectID.h"
#include "Object.h"
#include "ArcanaLog.h"

//vld
#include <vld.h>

namespace Arcana
{
	
	REGISTER_CATEGORY_ENGINE(ARCANA_RESOURCE_API, ResourceLog, none)
	
	class ARCANA_RESOURCE_API Resource : public Object
	{
	public:
	
		template<class T>
		class Type
		{
		public:
		
			static Resource* createFunction(const std::string& name, const std::string& type, const ResourceData& data)
			{
				return new T(name, type, data);
			}

			Type(std::string name)
			{
				ResourceManager::instance().addType(name, createFunction);
			}
		
		};
	
		Resource();
		
		Resource(const std::string& name, const std::string& type, const ResourceData& data);
		
		virtual ~Resource();
		
		
		const ResourceData& getData() const;
		
		const GlobalObjectID& getId() const;
		
		const std::string& getName() const;

		const std::string& getType() const;
		
		
		bool operator==(const Resource& other);
		
	private:
	
		GlobalObjectID _id;
		
		ResourceData _data;

		std::string _type;
	};
	
}

#endif // !RESOURCE_H_