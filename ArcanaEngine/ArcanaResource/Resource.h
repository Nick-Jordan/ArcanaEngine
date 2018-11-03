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
		
			static Resource* createFunction(const std::string& name, ResourceData& data)
			{
				return new T(name, data);
			}

			Type(std::string name)
			{
				ResourceManager::instance().addType(name, createFunction);
			}
		
		};
	
		Resource();
		
		Resource(const std::string& name, ResourceData& data);
		
		virtual ~Resource();
		
		
		ResourceData& getData();
		
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