#ifndef BASE_LIGHT_COMPONENT_H_
#define BASE_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"
#include "GlobalObjectID.h"
#include "RenderLight.h"
#include "LightType.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	class ARCANA_SCENE_API BaseLightComponent : public SceneComponent
	{
	public:

		/*struct ARCANA_SCENE_API Properties
		{
			bool SubsurfaceTransmission;
			bool CastVolumetricShadow;
			bool CastDynamicShadows;
			bool CastStaticShadows;
			float IndirectLightingIntensity;
			Color LightColor;
			float Intensity;
			float VolumetricScatteringIntensity;

			Properties();
		};*/

		BaseLightComponent();

		//BaseLightComponent(const Properties& properties);

		BaseLightComponent(const BaseLightComponent& copy);

		virtual ~BaseLightComponent();

		virtual LightType getLightType() const;

		virtual RenderLight createRenderLight();

		const UUID& getLightId() const;


		//const Properties& getProperties() const;

		//void setProperties(const Properties& properties);

		LinearColor getLightColor() const;

		float getIntensity() const;

		float getVolumetricScatteringIntensity() const;

		float getIndirectLightingIntensity() const;

		bool hasStaticShadows() const;

		bool hasDynamicShadows() const;

		bool hasVolumetricShadows() const;

		bool hasShadows() const;

		bool hasSubsurfaceTransmission() const;

		void setLightColor(const LinearColor& color);

		void setLightColor(const Color& color);

		void setIntensity(float intensity);

		void setVolumetricScatteringIntensity(float intensity);

		void setIndirectLightingIntensity(float intensity);

		void setStaticShadows(bool staticShadows);

		void setDynamicShadows(bool dynamicShadows);

		void setVolumetricShadows(bool volumetricShadows);

		void setSubsurfaceTransmission(bool subsurfaceTransmission);

		bool isStaticLight() const;

	private:

		UUID _lightId;
		//Properties _properties;

		bool _subsurfaceTransmission;
		bool _castVolumetricShadow;
		bool _castDynamicShadows;
		bool _castStaticShadows;
		float _indirectLightingIntensity;
		Color _lightColor;
		float _intensity;
		float _volumetricScatteringIntensity;
	};

	class BaseLightComponentResource : public ResourceCreator<BaseLightComponent>
	{
	public:

		BaseLightComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<BaseLightComponent>(id, type, data, dependencyScheduler)
		{
			initializeBaseLightComponent(this, data);
		}

		static void initializeBaseLightComponent(BaseLightComponent* light, const ResourceData& data)
		{
			light->setSubsurfaceTransmission(data.getBoolParameter("subsurfaceTransmission"));
			light->setVolumetricShadows(data.getBoolParameter("volumetricShadow") || data.getBoolParameter("castVoumetricShadow") || data.getBoolParameter("volumetricShadows") || data.getBoolParameter("castVoumetricShadows"));
			light->setDynamicShadows(data.getBoolParameter("dynamicShadow") || data.getBoolParameter("castDynamicShadow") || data.getBoolParameter("dynamicShadows") || data.getBoolParameter("castDynamicShadows"));
			light->setStaticShadows(data.getBoolParameter("staticShadow") || data.getBoolParameter("castStaticShadow") || data.getBoolParameter("staticShadows") || data.getBoolParameter("castStaticShadows"));
			light->setIndirectLightingIntensity(data.getFloatParameter("indirectLightingIntensity", 0.0f));

			std::string s = data.getStringParameter("color", "255, 255, 255");
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

			light->setLightColor(color);

			light->setIntensity(data.getFloatParameter("intensity", Math::PI));
			light->setVolumetricScatteringIntensity(data.getFloatParameter("volumetricScatteringIntensity", 0.0f));
		}
	};
}
#endif // !BASE_LIGHT_COMPONENT_H_
