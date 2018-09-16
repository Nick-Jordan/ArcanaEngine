#ifndef RESOURCE_CREATOR_H_
#define RESOURCE_CREATOR_H_

#include "Resource.h"

namespace Arcana
{

	template<class C>
	class ResourceCreator : public C, public Resource
	{
	public:

		ResourceCreator(const std::string& name, ResourceData& data);

		virtual ~ResourceCreator() {};
	};

	template<class C>
	inline ResourceCreator<C>::ResourceCreator(const std::string& name, ResourceData& data) :
		C(), Resource(name, data)
	{
	};

}

#endif // !RESOURCE_CREATOR_H_