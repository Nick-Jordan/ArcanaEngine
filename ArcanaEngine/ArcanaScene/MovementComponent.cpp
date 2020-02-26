#include "MovementComponent.h"

#include "Actor.h"

namespace Arcana
{

	MovementComponent::MovementComponent() : ActorComponent(), _sceneComponent(nullptr)
	{
	}


	MovementComponent::~MovementComponent()
	{
		
	}

	void MovementComponent::registered()
	{
		Actor* owner = getOwner();

		if (owner)
		{
			_sceneComponent = owner->getSceneComponent();
		}
	}

	SceneComponent* MovementComponent::getMovingSceneComponent() const
	{
		return _sceneComponent;
	}

	void MovementComponent::updateMovement(const Vector3d& delta, const Quaterniond& rotation)
	{
		if (_sceneComponent)
		{
			//constrain delta to plane
			_sceneComponent->translate(delta);
			_sceneComponent->rotate(rotation);
		}
	}
}
