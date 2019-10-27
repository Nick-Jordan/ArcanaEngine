#ifndef LIGHT_COMPONENT_H_
#define LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "BaseLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API LightComponent : public BaseLightComponent
	{
	public:

		struct Properties : public BaseLightComponent::Properties
		{
			bool AffectDynamicIndirectLighting;
			bool EnableLightShaftBloom;
			float BloomScale;
			float BloomThreshold;
			Color BloomTint;
			bool UseDistanceFieldShadows;
			bool UseTemperature;
			//translucent shadows?
			float ContactShadowLength;
			//contact length in world space/screen space
			float DisabledBrightness;
			float LightFunctionFadeDistance;
			//light function material?
			Vector3f LightFunctionScale;
			//light function channels
			float MaxDistanceFadeRange;
			float MaxDrawDistance;
			float RayStartOffsetDepthScale;
			float ShadowBias;
			float ShadowResolutionScale;
			float ShadowSharpen;
			float SpecularScale;
			//static shadow depth map
			float Temperature;
		};

		LightComponent();

		virtual ~LightComponent();

	private:

	};
}
#endif // !LIGHT_COMPONENT_H_
