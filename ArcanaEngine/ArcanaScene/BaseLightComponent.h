#ifndef BASE_LIGHT_COMPONENT_H_
#define BASE_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"
#include "GlobalObjectID.h"
#include "RenderLight.h"
#include "LightType.h"

namespace Arcana
{
	class ARCANA_SCENE_API BaseLightComponent : public SceneComponent
	{
	public:

		BaseLightComponent();

		virtual ~BaseLightComponent();

		virtual LightType getLightType() const;

		virtual RenderLight createRenderLight();

	private:

		bool _castVolumetricShadow;
		bool _subsurfaceTransmission;
		bool _castDynamicShadows;
		bool _castStaticShadows;
		bool _castShadows;
		float _indirectLightingIntensity;
		Vector3f _lightColor;//Color class?
		GlobalObjectID _lightId;
		float _volumetricScatteringIntensity;
	};
}
#endif // !BASE_LIGHT_COMPONENT_H_
