#include "PointLightComponent.h"

namespace Arcana
{

	PointLightComponent::PointLightComponent()
	{
	}


	PointLightComponent::~PointLightComponent()
	{
	}

	LightType PointLightComponent::getLightType() const
	{
		return Point;
	}

	RenderLight PointLightComponent::createRenderLight()
	{
		RenderLight light;

		Vector3d position = getWorldPosition();

		light.position = position.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.type = (int32) Point;

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}
}
