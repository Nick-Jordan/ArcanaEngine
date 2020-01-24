#include "AreaLightComponent.h"

namespace Arcana
{

	AreaLightComponent::AreaLightComponent()
	{
		//test
		_sourceWidth = 1.0;
		_sourceHeight = 1.0;
	}

	AreaLightComponent::~AreaLightComponent()
	{
	}

	LightType AreaLightComponent::getLightType() const
	{
		return Area;
	}

	Vector4d AreaLightComponent::getLightPosition()
	{
		return Vector4d(getWorldPosition(), 1.0);
	}

	RenderLight AreaLightComponent::createRenderLight()
	{
		RenderLight light;

		Vector3d position = getLightPosition().xyz();
		Vector3d direction = getLightDirection();

		light.position = position.cast<float>();
		light.direction = direction.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.sourceWidth = _sourceWidth;
		light.sourceHeight = _sourceHeight;
		light.type = Area;
		light.mobility = isStaticLight();
		light.id = getLightId();
		//light shaft cone angle

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}
}