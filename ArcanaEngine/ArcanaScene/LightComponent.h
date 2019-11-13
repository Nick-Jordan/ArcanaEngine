#ifndef LIGHT_COMPONENT_H_
#define LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "BaseLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API LightComponent : public BaseLightComponent
	{
	public:

		/*struct ARCANA_SCENE_API Properties : public BaseLightComponent::Properties
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
		};*/

		LightComponent();

		LightComponent(const LightComponent& lightComponent);

		virtual ~LightComponent();

		virtual Vector4d getLightPosition();

		Vector3d getLightDirection();


		bool getDynamicIndirectLighting() const;

		bool getLightShaftBloomEnabled() const;

		float getBloomScale() const;

		float getBloomThreshold() const;

		const Color& getBloomTint() const;

		bool getUseDistanceFieldShadows() const;

		bool getUseTemperature() const;
		
		float getContactShadowLength() const;
		
		float getDisabledBrightness() const;

		float getLightFunctionFadeDistance() const;

		const Vector3f& getLightFunctionScale() const;

		float getMaxDistanceFadeRange() const;

		float getMaxDrawDistance() const;

		float getRayStartOffsetDepthScale() const;

		float getShadowBias() const;

		float getShadowResolutionScale() const;

		float getShadowSharpen() const;

		float getSpecularScale() const;

		float getTemperature() const;


		void setDynamicIndirectLighting(bool affectDynamicIndirectLighting);

		void setLightShaftBloomEnabled(bool enableLightShaftBloom);

		void setBloomScale(float bloomScale);

		void setBloomThreshold(float bloomThreshold);

		void setBloomTint(const Color& color);

		void setUseDistanceFieldShadows(bool distanceFieldShadows);

		void setUseTemperature(bool useTemperature);

		void setContactShadowLength(float contactShadowLength);

		void setDisabledBrightness(float disabledBrightness);

		void setLightFunctionFadeDistance(float lightFunctionFadeDistance);

		void setLightFunctionScale(const Vector3f& lightFunctionScale);

		void setMaxDistanceFadeRange(float maxDistanceFadeRange);

		void setMaxDrawDistance(float maxDrawDistance);

		void setRayStartOffsetDepthScale(float rayStartOffsetDepthScale);

		void setShadowBias(float shadowBias);

		void setShadowResolutionScale(float shadowResolutionScale);

		void setShadowSharpen(float shadowSharpen);

		void setSpecularScale(float specularScale);

		void setTemperature(float temperature);

	private:

		bool _affectDynamicIndirectLighting;
		bool _enableLightShaftBloom;
		float _bloomScale;
		float _bloomThreshold;
		Color _bloomTint;
		bool _useDistanceFieldShadows;
		bool _useTemperature;
		float _contactShadowLength;
		float _disabledBrightness;
		float _lightFunctionFadeDistance;
		Vector3f _lightFunctionScale;
		float _maxDistanceFadeRange;
		float _maxDrawDistance;
		float _rayStartOffsetDepthScale;
		float _shadowBias;
		float _shadowResolutionScale;
		float _shadowSharpen;
		float _specularScale;
		float _temperature;
	};
}
#endif // !LIGHT_COMPONENT_H_
