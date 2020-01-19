#include "LightProperties.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	class LightPropertiesResource : public ResourceCreator<LightProperties>
	{
	public:

		LightPropertiesResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<LightProperties>(id, type, data)
		{
			CastsDynamicShadow = data.getBoolParameter("castsDynamicShadow");
		}
	};

	Resource::Type<LightPropertiesResource> lightPropertiesResource("lightProperties");
}