#ifndef RESOURCE_DATABASE_H_
#define RESOURCE_DATABASE_H_

#include "ResourceDefines.h"

#include "Types.h"
#include "Resource.h"

namespace Arcana
{
	
	class ARCANA_RESOURCE_API ResourceDatabase
	{
	public:
			
		ResourceDatabase();
		
		ResourceDatabase(const std::string& filename);
		
		~ResourceDatabase();
		
		
		bool initialize(const std::string& filename);
		
		const Resource& getResource(const std::string& name);
		
		const Resource& getResource(const GlobalObjectID& id);
		
	private:
		
		Array<Resource> _resources;
	};
	
}

#endif // !RESOURCE_DATABASE_H_