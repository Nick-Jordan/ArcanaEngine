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

	Vector4d PointLightComponent::getLightPosition()
	{
		return Vector4d(getWorldPosition(), 1.0);
	}

	RenderLight PointLightComponent::createRenderLight()
	{
		RenderLight light;

		Vector3d position = getLightPosition().xyz();

		light.position = position.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.type = Point;
		light.mobility = isStaticLight();

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}
}
