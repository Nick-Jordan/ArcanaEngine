#include "BaseLightComponent.h"

namespace Arcana
{

	BaseLightComponent::BaseLightComponent()
	{
	}


	BaseLightComponent::~BaseLightComponent()
	{
	}

	LightType BaseLightComponent::getLightType() const
	{
		return NumLightTypes;
	}

	RenderLight BaseLightComponent::createRenderLight() const
	{
		return RenderLight();
	}
}
