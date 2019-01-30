#include "RectLightComponent.h"

namespace Arcana
{

	RectLightComponent::RectLightComponent()
	{
	}


	RectLightComponent::~RectLightComponent()
	{
	}

	LightType RectLightComponent::getLightType() const
	{
		return Rectangle;
	}
}
