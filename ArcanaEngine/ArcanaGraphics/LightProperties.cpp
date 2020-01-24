#include "LightProperties.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	class LightPropertiesResource : public ResourceCreator<LightProperties>
	{
	public:

		LightPropertiesResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<LightProperties>(id, type, data, dependencyScheduler)
		{
			CastsDynamicShadow = data.getBoolParameter("castsDynamicShadow");
		}
	};

	Resource::Type<LightPropertiesResource> lightPropertiesResource("lightProperties");
}