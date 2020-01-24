#ifndef SPOT_LIGHT_COMPONENT_H_
#define SPOT_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "PointLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API SpotLightComponent : public PointLightComponent
	{
	public:

		SpotLightComponent();

		virtual ~SpotLightComponent();

		virtual LightType getLightType() const override;

		virtual Vector4d getLightPosition() override;

		virtual RenderLight createRenderLight() override;

		float getInnerConeAngle() const;

		void setInnerConeAngle(float innerConeAngle);

		float getOuterConeAngle() const;

		void setOuterConeAngle(float outerConeAngle);

		float getLightShaftConeAngle() const;

		void setLightShaftConeAngle(float lightShaftConeAngle);

	private:

		float _innerConeAngle;
		float _lightShaftConeAngle;
		float _outerConeAngle;
	};

	class SpotLightComponentResource : public ResourceCreator<SpotLightComponent>
	{
	public:

		SpotLightComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<SpotLightComponent>(id, type, data, dependencyScheduler)
		{
			initializeSpotLightComponent(this, data);
		}

		static void initializeSpotLightComponent(SpotLightComponent* light, const ResourceData& data)
		{
			PointLightComponentResource::initializePointLightComponent(light, data);

			light->setInnerConeAngle(data.getFloatParameter("innerConeAngle", Math::degreesToRadians(12.0f)));
			light->setOuterConeAngle(data.getFloatParameter("outerConeAngle", Math::degreesToRadians(15.0f)));
			light->setLightShaftConeAngle(data.getFloatParameter("lightShaftConeAngle"));
		}
	};
}
#endif // !SPOT_LIGHT_COMPONENT_H_
