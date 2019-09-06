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
		light.color = Vector3f::one() * 50.0;
		light.type = (int32) Point;

		light.lightProperties.CastsDynamicShadow = true;

		return light;
	}
}
