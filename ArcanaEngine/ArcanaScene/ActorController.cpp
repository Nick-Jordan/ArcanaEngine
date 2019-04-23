#include "ActorController.h"

namespace Arcana
{

	ActorController::ActorController(const std::string& id) 
		: Actor(id), _attachToActor(false), _useLookInput(true), _useMovementInput(true)
	{
		initialize(id);
	}


	ActorController::~ActorController()
	{
	}

	void ActorController::initialize(std::string name, const Actor* templateActor)
	{
		Actor::initialize(name, templateActor);

		getSceneComponent()->useAbsoluteRotation(true);
		setVisible(false);
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

	void ActorController::attach(Actor* actor)
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

	Actor* ActorController::getActor() const
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

	void ActorController::control(Actor* actor)
	{
		if (actor != nullptr)
		{
			if (getActor() && getActor() != actor)
			{
				releaseControl();
			}

			if (actor->getController() != nullptr)
			{
				actor->getController()->releaseControl();
			}

			actor->setController(this);
			_controllingActor = actor;
			_controlActorCallback.executeIfBound(actor);
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
