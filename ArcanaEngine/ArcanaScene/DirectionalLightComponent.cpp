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

	RenderLight  DirectionalLightComponent::createRenderLight() const
	{
		RenderLight light;

		Vector3d position = getWorldPosition();

		light.position = position.cast<float>();
		light.color = Vector3f::one() * 100.0;
		light.type = (int32)Directional;

		return light;
	}
}