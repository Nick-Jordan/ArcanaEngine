#ifndef LOCAL_LIGHT_COMPONENT_H_
#define LOCAL_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "LightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API LocalLightComponent : public LightComponent
	{
	public:

		LocalLightComponent();

		virtual ~LocalLightComponent();

	private:

		float _attenuationRadius;
		//light intensity units
		//ftl point light settings
	};

	class LocalLightComponentResource : public ResourceCreator<LocalLightComponent>
	{
	public:

		LocalLightComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<LocalLightComponent>(id, type, data, dependencyScheduler)
		{
			initializeLocalLightComponent(this, data);
		}

		static void initializeLocalLightComponent(LocalLightComponent* light, const ResourceData& data)
		{
			LightComponentResource::initializeLightComponent(light, data);

			//add stuff
		}
	};
}
#endif // !LOCAL_LIGHT_COMPONENT_H_
