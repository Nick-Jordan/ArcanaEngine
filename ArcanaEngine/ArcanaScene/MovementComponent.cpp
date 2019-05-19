#include "MovementComponent.h"

namespace Arcana
{

	MovementComponent::MovementComponent()
	{
	}


	MovementComponent::~MovementComponent()
	{
	}

	SceneComponent* MovementComponent::getMovingSceneComponent() const
	{
		return _sceneComponent;
	}
}
