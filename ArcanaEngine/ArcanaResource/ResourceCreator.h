#ifndef RESOURCE_CREATOR_H_
#define RESOURCE_CREATOR_H_

#include "Resource.h"

#include "GlobalObjectID.h"

namespace Arcana
{

	template<class C>
	class ResourceCreator : public C, public Resource
	{
	public:

		ResourceCreator(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler);

		virtual ~ResourceCreator() {};
	};

	template<class C>
	inline ResourceCreator<C>::ResourceCreator(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler) :
		C(), Resource(id, type, data)
	{
	};

}

#endif // !RESOURCE_CREATOR_H_