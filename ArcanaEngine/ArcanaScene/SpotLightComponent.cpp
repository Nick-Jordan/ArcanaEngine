#include "SpotLightComponent.h"

namespace Arcana
{

	SpotLightComponent::SpotLightComponent() : PointLightComponent()
	{
		//test
		_innerConeAngle = Math::degreesToRadians(12.0f);
		_outerConeAngle = Math::degreesToRadians(15.0f);
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
		Vector3d direction = getLightDirection();

		light.position = position.cast<float>();
		light.direction = direction.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.sourceRadius = getSourceRadius();
		light.type = Spot;
		light.mobility = isStaticLight();
		light.innerAngle = _innerConeAngle;
		light.outerAngle = _outerConeAngle;
		light.constant = getConstantAttenuation();
		light.linear = getLinearAttenuation();
		light.quadratic = getQuadraticAttenuation();
		light.id = getLightId().getId();
		//light shaft cone angle

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}
}