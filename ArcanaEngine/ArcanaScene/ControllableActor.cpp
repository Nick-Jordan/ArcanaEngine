#include "ControllableActor.h"

namespace Arcana
{
	double ControllableActor::speed = 100.0;

	ControllableActor::ControllableActor() : Actor(), _controller(nullptr)
	{

	}

	ControllableActor::ControllableActor(const std::string& id) : Actor(id), _controller(nullptr)
	{

	}

	ControllableActor::~ControllableActor()
	{

	}


	void ControllableActor::update(double elapsedTime)
	{
		Actor::update(elapsedTime);

		Vector3f input = resetMovementInput();

		float xAxis = input.x;
		float yAxis = input.z;
		float upAxis = input.y;

		if (abs(xAxis) > 0.05 || abs(yAxis) > 0.05 || abs(upAxis) > 0.05)
		{
			xAxis = abs(xAxis) < 0.05 ? 0.0f : xAxis;
			yAxis = abs(yAxis) < 0.05 ? 0.0f : yAxis;
			upAxis = abs(upAxis) < 0.05 ? 0.0f : upAxis;

			SceneComponent* comp = getSceneComponent();

			/*Vector3d finalVector = comp->getForwardVector() * -yAxis
				+ comp->getUpVector() * upAxis
				+ comp->getRightVector() * xAxis;*/

			Vector3d finalVector = Vector3d(xAxis, upAxis, yAxis);

			finalVector.normalize();

			double speed = ControllableActor::speed;//temp
			Vector3d movement = finalVector * elapsedTime * speed;

			comp->translate(movement);//speed>?????????????
		}
	}


	ActorController* ControllableActor::getController() const
	{
		return _controller;
	}

	OnControlledCallback& ControllableActor::getOnControlledCallback()
	{
		return _controlledCallback;
	}

	void ControllableActor::setController(ActorController* controller)
	{
		_controller = controller;
	}


	void ControllableActor::addMovementInput(Vector3f direction, float input)//bool force???
	{
		ActorMovementComponent* c = getMovementComponent();
		if (c)
		{
			return c->addMovementInput(direction * input);// , bForce);
		}

		defaultAddMovementInput(direction * input);
	}

	ActorMovementComponent* ControllableActor::getMovementComponent() const
	{
		return getComponent<ActorMovementComponent>();
	}

	Vector3f ControllableActor::resetMovementInput()
	{
		ActorMovementComponent* c = getMovementComponent();
		if (c)
		{
			return c->resetMovementInput();
		}

		return defaultResetMovementInput();
	}

	void ControllableActor::defaultAddMovementInput(Vector3f input)
	{
		if (isInputEnabled()) // || force
		{
			_movementInput += input;
		}
	}

	Vector3f ControllableActor::defaultResetMovementInput()
	{
		_lastMovementInput = _movementInput;
		_movementInput = Vector3f::zero();
		return _lastMovementInput;
	}

	Vector3f ControllableActor::defaultGetLastMovementInput()
	{
		return _lastMovementInput;
	}

	Vector3f ControllableActor::defaultGetCurrentMovementInput()
	{
		return _movementInput;
	}
}