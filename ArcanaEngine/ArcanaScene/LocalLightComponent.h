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
}
#endif // !LOCAL_LIGHT_COMPONENT_H_
