#include "DirectionalLightComponent.h"

namespace Arcana
{

	DirectionalLightComponent::DirectionalLightComponent()
	{
	}


	DirectionalLightComponent::~DirectionalLightComponent()
	{
	}

	LightType DirectionalLightComponent::getLightType() const
	{
		return Directional;
	}

	RenderLight DirectionalLightComponent::createRenderLight()
	{
		RenderLight light;

		Vector3d direction = getLightDirection();

		light.direction = direction.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.type = Directional;
		light.mobility = isStaticLight();

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}

	Resource::Type<DirectionalLightComponentResource> directionalLightComponentResource("directionalLightComponent");
}