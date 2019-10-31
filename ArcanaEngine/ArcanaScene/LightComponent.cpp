#include "LightComponent.h"

namespace Arcana
{

	LightComponent::LightComponent()
	{
	}


	LightComponent::~LightComponent()
	{
	}

	Vector4d LightComponent::getLightPosition()
	{
		return Vector4d::zero();
	}

	Vector3d LightComponent::getLightDirection()
	{
		return getWorldTransform().getForwardVector();
	}
}