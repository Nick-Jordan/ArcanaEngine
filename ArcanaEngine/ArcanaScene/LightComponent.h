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

	class LightComponentResource : public ResourceCreator<LightComponent>
	{
	public:

		LightComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<LightComponent>(id, type, data, dependencyScheduler)
		{
			initializeLightComponent(this, data);
		}

		static void initializeLightComponent(LightComponent* light, const ResourceData& data)
		{
			BaseLightComponentResource::initializeBaseLightComponent(light, data);

			light->setDynamicIndirectLighting(data.getBoolParameter("dynamicIndirectLighting") || data.getBoolParameter("affectsDynamicIndirectLighting") || data.getBoolParameter("affectDynamicIndirectLighting"));
			light->setLightShaftBloomEnabled(data.getBoolParameter("enabledLightShaftBloom") || data.getBoolParameter("lightShaftBloom"));//true default
			light->setBloomScale(data.getFloatParameter("bloomScale", 1.0f));
			light->setBloomThreshold(data.getFloatParameter("bloomThreshold", 0.01f));
			light->setUseDistanceFieldShadows(data.getBoolParameter("useDistanceFieldShadows") || data.getBoolParameter("distanceFieldShadows"));
			light->setUseTemperature(data.getBoolParameter("useTemperature"));
			light->setContactShadowLength(data.getFloatParameter("contactShadowLength", 1.0f));
			light->setDisabledBrightness(data.getFloatParameter("disabledBrightness", 0.0f));
			light->setLightFunctionFadeDistance(data.getFloatParameter("lightFunctionFadeDistance", 1.0f));
			light->setMaxDistanceFadeRange(data.getFloatParameter("maxDistanceFadeRange", 0.0f));
			light->setMaxDrawDistance(data.getFloatParameter("rayStartOffsetDepthScale", 0.0f));
			light->setRayStartOffsetDepthScale(data.getFloatParameter("lightFunctionFadeDistance"));
			light->setShadowBias(data.getFloatParameter("shadowBias", 0.15f));//0.5f
			light->setShadowResolutionScale(data.getFloatParameter("shadowResolutionScale", 1.0f));
			light->setShadowSharpen(data.getFloatParameter("shadowSharpen", 0.0f));
			light->setSpecularScale(data.getFloatParameter("specularScale", 1.0f));
			light->setTemperature(data.getFloatParameter("temperature", 1000.0f));

			std::string s = data.getStringParameter("bloomTint", "255, 255, 255");
			Color color;

			size_t pos = s.find(",");
			color.R = stoul(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			color.G = stoul(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			color.B = stoul(s.substr(0, pos));
			s.erase(0, pos + 1);

			light->setBloomTint(color);

			s = data.getStringParameter("lightFunctionScale", "1.0f, 1.0f, 1.0f");
			Vector3f vec;

			pos = s.find(",");
			vec.x = stof(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			vec.y = stof(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			vec.z = stof(s.substr(0, pos));
			s.erase(0, pos + 1);

			light->setLightFunctionScale(vec);
		}
	};
}
#endif // !LIGHT_COMPONENT_H_
