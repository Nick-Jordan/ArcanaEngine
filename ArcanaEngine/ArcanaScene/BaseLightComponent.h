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

		const GlobalObjectID& getLightId() const;


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

		GlobalObjectID _lightId;
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
}
#endif // !BASE_LIGHT_COMPONENT_H_
