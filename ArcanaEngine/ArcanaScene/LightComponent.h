#ifndef LIGHT_COMPONENT_H_
#define LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "BaseLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API LightComponent : public BaseLightComponent
	{
	public:

		LightComponent();

		virtual ~LightComponent();

	private:

		bool _affectDynamicIndirectLighting;
		bool _enableLightShaftBloom;
		//force cached shadows
		float _bloomScale;
		float _bloomThreshold;
		Vector3f _bloomTint;//Color?
		bool _useDistanceFieldShadows;
		bool _useTemperature;
		//translucent shadows?
		float _contactShadowLength;
		//contact length in world space/screen space
		float _disabledBrightness;
		float _lightFunctionFadeDistance;
		//light function material?
		Vector3f _lightFunctionScale;
		//light function channels
		float _maxDistanceFadeRange;
		float _maxDrawDistance;
		float _rayStartOffsetDepthScale;
		float _shadowBias;
		float _shadowResolutionScale;
		float _shadowSharpen;
		float _specularScale;
		//static shadow depth map
		float _temperature;
	};
}
#endif // !LIGHT_COMPONENT_H_
