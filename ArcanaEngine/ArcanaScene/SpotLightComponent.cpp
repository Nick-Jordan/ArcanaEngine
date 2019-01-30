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
}