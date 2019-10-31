#include "SpotLightComponent.h"

namespace Arcana
{

	SpotLightComponent::SpotLightComponent()
	{
	}


	SpotLightComponent::~SpotLightComponent()
	{
	}

	LightType SpotLightComponent::getLightType() const
	{
		return Spot;
	}

	Vector4d SpotLightComponent::getLightPosition()
	{
		return Vector4d(getWorldPosition(), 1.0);
	}

	RenderLight SpotLightComponent::createRenderLight()
	{
		RenderLight light;

		Vector3d position = getLightPosition().xyz();

		light.position = position.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.type = Point;
		light.mobility = isStaticLight();
		light.innerAngle = _innerConeAngle;
		light.outerAngle = _outerConeAngle;
		//light shaft cone angle

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}
}