#ifndef LIGHT_COMPONENT_H_
#define LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "BaseLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API LightComponent : public BaseLightComponent
	{
	public:

		struct ARCANA_SCENE_API Properties : public BaseLightComponent::Properties
		{
			bool AffectDynamicIndirectLighting;
			bool EnableLightShaftBloom;
			float BloomScale;
			float BloomThreshold;
			Color BloomTint;
			bool UseDistanceFieldShadows;
			bool UseTemperature;
			float ContactShadowLength;
			float DisabledBrightness;
			float LightFunctionFadeDistance;
			Vector3f LightFunctionScale;
			float MaxDistanceFadeRange;
			float MaxDrawDistance;
			float RayStartOffsetDepthScale;
			float ShadowBias;
			float ShadowResolutionScale;
			float ShadowSharpen;
			float SpecularScale;
			float Temperature;

			Properties();
		};

		LightComponent();

		virtual ~LightComponent();

		virtual Vector4d getLightPosition();

		Vector3d getLightDirection();

	private:

	};
}
#endif // !LIGHT_COMPONENT_H_
