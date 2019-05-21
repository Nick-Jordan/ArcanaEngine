#include "ActorMovementComponent.h"

#include "ControllableActor.h"

namespace Arcana
{

	ActorMovementComponent::ActorMovementComponent() : _owner(nullptr)
	{
	}


	ActorMovementComponent::~ActorMovementComponent()
	{
	}

	void ActorMovementComponent::addMovementInput(Vector3f input)
	{
		if (_owner)
		{
			_owner->defaultAddMovementInput(input);
		}
	}

	Vector3f ActorMovementComponent::resetMovementInput()
	{
		if (_owner)
		{
			return _owner->defaultResetMovementInput();
		}

		return Vector3f::zero();
	}

	Vector3f ActorMovementComponent::getLastMovementInput() const
	{
		if (_owner)
		{
			//return _owner->defaultGetLastMovementInput();
		}

		return Vector3f::zero();
	}

	Vector3f ActorMovementComponent::getCurrentMovementInput() const
	{
		if (_owner)
		{
			//return _owner->defaultGetCurrentMovementInput();
		}
		
		return Vector3f::zero();
	}

	ControllableActor* ActorMovementComponent::getControllableActor() const
	{
		return _owner;
	}

	bool ActorMovementComponent::isInputEnabled() const
	{
		//moving component????
		if (getMovingSceneComponent() && _owner)
		{
			return _owner->isInputEnabled();
		}

		return false;
	}

}
