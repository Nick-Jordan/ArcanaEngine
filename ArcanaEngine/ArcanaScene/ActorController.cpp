#include "ActorController.h"

namespace Arcana
{
	ActorController::ActorController()
		: Actor(), _attachToActor(false), _useLookInput(true), _useMovementInput(true), _controllingActor(nullptr)
	{
	}

	ActorController::ActorController(const std::string& id) 
		: Actor(id), _attachToActor(false), _useLookInput(true), _useMovementInput(true), _controllingActor(nullptr)
	{
	}


	ActorController::~ActorController()
	{
	}

	void ActorController::initialize(std::string name, const Actor* templateActor)
	{
		Actor::initialize(name, templateActor);

		getSceneComponent()->useAbsoluteRotation(true);
		setVisible(false);

		//getWorld()->addController(this); not here
	}

	void ActorController::update(double elapsedTime)
	{

	}

	void ActorController::destroyed()
	{
		releaseControl();
		//getWorld()->removeController(this);
		Actor::destroyed();
	}

	void ActorController::attach(ControllableActor* actor)
	{
		if (_attachToActor && getSceneComponent())
		{
			if (actor)
			{
				if (actor->getSceneComponent() && getSceneComponent()->getAttachParent() != actor->getSceneComponent())
				{
					getSceneComponent()->detach(false);
					getSceneComponent()->setPosition(Vector3d::zero());
					getSceneComponent()->setRotation(Quaterniond::IDENTITY);
					getSceneComponent()->attach(actor->getSceneComponent());
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
		if (_attachToActor && getSceneComponent() && getSceneComponent()->getAttachParent())
		{
			getSceneComponent()->detach(true);
		}
	}

	void ActorController::setControllerRotation(const Quaterniond& rotation)
	{
		if (_controllerRotation != rotation)
		{
			_controllerRotation = rotation;
			if (getSceneComponent() && getSceneComponent()->hasAbsoluteRotation())
			{
				getSceneComponent()->setRotation(getControllerRotation());
			}

			if (_controllingActor)
			{
				_controllingActor->getLocalTransform().setRotation(_controllerRotation);
			}
		}
	}
	const Quaterniond& ActorController::getControllerRotation() const
	{
		return _controllerRotation;
	}

	const Quaterniond& ActorController::getDesiredRotation() const
	{
		return getControllerRotation();
	}

	ControlActorCallback& ActorController::getOnControlActorCallback()
	{
		return _controlActorCallback;
	}

	void ActorController::getControllerViewPoint(Vector3d& location, Quaterniond& rotation)
	{
		if (_controllingActor)
		{
			_controllingActor->getViewPoint(location, rotation);
		}
	}

	ControllableActor* ActorController::getControllingActor() const
	{
		return _controllingActor;
	}

	bool ActorController::isUsingLookInput() const
	{
		return _useLookInput;
	}

	void ActorController::setUseLookInput(bool useLookInput)
	{
		_useLookInput = useLookInput;
	}

	bool ActorController::isUsingMovementInput() const
	{
		return _useMovementInput;
	}

	void ActorController::setUseMovementInput(bool useMovementInput)
	{
		_useMovementInput = useMovementInput;
	}

	bool ActorController::isPlayerController() const
	{
		return false;
	}

	void ActorController::control(ControllableActor* actor)
	{
		if (actor != nullptr)
		{
			if (getControllingActor() && getControllingActor() != actor)
			{
				releaseControl();
			}

			if (actor->getController() != nullptr)
			{
				actor->getController()->releaseControl();
			}

			actor->setController(this);

			setControllerRotation(actor->getTransform().getRotation());

			_controllingActor = actor;
			_controlActorCallback.executeIfBound(actor);
			actor->getOnControlledCallback().executeIfBound(this);
		}
	}

	void ActorController::releaseControl()
	{
		if (_controllingActor != nullptr)
		{
			_controllingActor->setController(nullptr);
			_controllingActor = nullptr;
		}
	}
}
