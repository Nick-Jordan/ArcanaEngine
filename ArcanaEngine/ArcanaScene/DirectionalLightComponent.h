#ifndef DIRECTIONAL_LIGHT_COMPONENT_H_
#define DIRECTIONAL_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "LightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API DirectionalLightComponent : public LightComponent
	{
	public:

		DirectionalLightComponent();

		virtual ~DirectionalLightComponent();

		virtual LightType getLightType() const override;

		virtual RenderLight createRenderLight() override;

	private:

		bool _enableLightShaftOcclusion;
		bool _usedAsAtmosphereSunLight;
		bool _useInsetShadowsForMovableObjects;
		float _cascadeDistributionExponent;
		float _cascadeTransitionFraction;
		float _distanceFieldShadowDistance;
		int32 _dynamicShadowCascades;
		float _dynamicShadowDistanceMovableLight;
		float _dynamicShadowDistanceStationaryLight;
		int32 _farShadowCascadeCount;
		float _farShadowDistance;
		//ftl directional light settings;
		Vector3f _lightShaftOverrideDirection;
		float _lightSourceAngle;
		float _lightSourceSoftAngle;
		Color _modulatedShadowColor;
		float _occlusionDepthRange;
		float _occlusionMaskDarkness;
		float _shadowDistanceFadeoutFraction;
		float _traceDistance;
	};

	class DirectionalLightComponentResource : public ResourceCreator<DirectionalLightComponent>
	{
	public:

		DirectionalLightComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<DirectionalLightComponent>(id, type, data, dependencyScheduler)
		{
			LightComponentResource::initializeLightComponent(this, data);

			//directionalLightStuff
		}
	};
}
#endif // !DIRECTIONAL_LIGHT_COMPONENT_H_
