/*#include "ActorController.h"

namespace Arcana
{

	ActorController::ActorController(const std::string& id) 
		: Actor(id), _isPlayerController(false), _attachToActor(false), _useLookInput(true), _useMovementInput(true)
	{
		getSceneComponent()->useAbsoluteRotation(true);
		setVisible(false);
	}


	ActorController::~ActorController()
	{
	}

	void ActorController::attach(Actor* actor)
	{
		if (_attachToActor && getSceneComponent())
		{
			if (actor)
			{
				/*if (actor->getSceneComponent() && getSceneComponent()->GetAttachParent() != InPawn->GetRootComponent())
				{
					RootComponent->DetachFromParent(false);
					RootComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
					RootComponent->AttachTo(InPawn->GetRootComponent());
				}
			}
			else
			{
				detach();
			}
		}
	}

	void ActorController::detach()
	{
		//if (_attachToActor && getSceneComponent() && getSceneComponent()->GetAttachParent() && Cast<Actor>(RootComponent->GetAttachmentRootActor()))
		{
			//getSceneComponent()->DetachFromParent(true);
		}
	}

	//get character?

	void ActorController::setControllerRotation(const Quaterniond& rotation)
	{
		if (_controllerRotation != rotation)
		{
			_controllerRotation = rotation;

			if (getSceneComponent() && getSceneComponent()->hasAbsoluteRotation())
			{
				getSceneComponent()->setRotation(getControllerRotation());
			}
		}
	}

	const Quaterniond& ActorController::getControllerRotation() const
	{
		return _controllerRotation;
	}

	const Quaterniond& ActorController::getDesiredRotation() const;

	ControlActorCallback& ActorController::getOnControlActorCallback();

	Actor* ActorController::getActor() const;

	//get player state?

	void ActorController::getControllerViewPoint(Vector3d& location, Quaterniond& rotation);

	//get state name?

	//get scene component? (root scene component from actor class)

	Actor* ActorController::getViewTarget() const
	{
		if (_actor)
		{
			return _actor;
		}

		return const_cast<ActorController*>(this);
	}

	//nagivation control?

	//state management (is in state)

	//is local controller / is local player controller

	bool ActorController::usesLookInput() const;

	bool ActorController::usesMovementInput() const;

	bool ActorController::isPlayerController() const;

	bool ActorController::hasClearLineOfSight(const Actor* actor, Vector3d position, bool alternateChecks)
	{
		return false;
	}

	void ActorController::control(Actor* actor);

	void ActorController::setControllerRotation(const Quaterniond& rotation);

	void ActorController::setUseLookInput(bool input);

	void ActorController::setUseMovementInput(bool input);

	//stop movement?

	void ActorController::releaseControl();

	void ActorController::update(double elapsedTime);
}
*/