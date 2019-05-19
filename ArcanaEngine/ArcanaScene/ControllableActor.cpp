#include "ControllableActor.h"

#include "PlayerController.h"

namespace Arcana
{

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
		float yRotation = 0.0;// rotation.y;
		float xRotation = 0.0;// rotation.x;
		float zRotation = 0.0;// rotation.z;
		//rotation.x = rotation.y = 0.0f;

		if (abs(xAxis) > 0.05 || abs(yAxis) > 0.05 || abs(upAxis) > 0.05)
		{
			xAxis = abs(xAxis) < 0.05 ? 0.0f : xAxis;
			yAxis = abs(yAxis) < 0.05 ? 0.0f : yAxis;
			upAxis = abs(upAxis) < 0.05 ? 0.0f : upAxis;

			SceneComponent* comp = getSceneComponent();

			/*Vector3d finalVector = comp->getForwardVector() * -yAxis
				+ comp->getUpVector() * upAxis
				+ comp->getRightVector() * xAxis;*/

			Vector3d finalVector = Vector3d(xAxis, upAxis, -yAxis);

			finalVector.normalize();

			Vector3d movement = finalVector * elapsedTime * speed;

			comp->translate(movement);//speed>?????????????
		}

		/*if (abs(xRotation) > 0.02 || abs(yRotation) > 0.02 || abs(zRotation) > 0.02)
		{
			xRotation = abs(xRotation) < 0.02 ? 0.0f : xRotation;
			yRotation = abs(yRotation) < 0.02 ? 0.0f : yRotation;
			zRotation = abs(zRotation) < 0.02 ? 0.0f : zRotation;

			double speed = 100.0;

			SceneComponent* comp = getSceneComponent();

			Quaterniond quatY;
			quatY.fromAxisAngle(Vector3d::unitY(), -yRotation * elapsedTime * speed);
			cameraComponents[0]->rotate(quatY);

			Quaterniond quatX;
			quatX.fromAxisAngle(Vector3d::unitX(), xRotation * elapsedTime * speed);
			cameraComponents[0]->rotate(quatX);

			Quaterniond quatZ;
			quatZ.fromAxisAngle(Vector3d::unitZ(), zRotation * elapsedTime * speed);
			cameraComponents[0]->rotate(quatZ);
		}*/
	}


	void ControllableActor::addPitchInput(float input)
	{
		if (input != 0.0f && _controller && _controller->isLocalPlayerController())
		{
			PlayerController* c = dynamic_cast<PlayerController*>(_controller);
			if (c)
			{
				c->addPitchInput(input);
			}
		}
	}

	void ControllableActor::addRollInput(float input)
	{
		if (input != 0.0f && _controller && _controller->isLocalPlayerController())
		{
			PlayerController* c = dynamic_cast<PlayerController*>(_controller);
			if (c)
			{
				c->addRollInput(input);
			}
		}
	}

	void ControllableActor::addYawInput(float input)
	{
		if (input != 0.0f && _controller && _controller->isLocalPlayerController())
		{
			PlayerController* c = dynamic_cast<PlayerController*>(_controller);
			if (c)
			{
				c->addYawInput(input);
			}
		}
	}

	void ControllableActor::addMovementInput(Vector3f direction, float input)//bool force???
	{
		ActorMovementComponent* c = getMovementComponent();
		if (c)
		{
			c->addMovementInput(direction * input);// , bForce);
		}
		
		defaultAddMovementInput(direction * input);
	}

	Vector3f ControllableActor::resetMovementInput()
	{
		/*LOGF(Info, CoreEngine, "movementInput: %f, %f, %f.... totalInput: %f, %f, %f", _movementInput.x, _movementInput.y, _movementInput.z, _totalControlInput.x, _totalControlInput.y, _totalControlInput.z);

		if (Math::abs(_totalControlInput.x) >= Math::abs(_movementInput.x))
		{
			_movementInput.x = 0.0f;
			_totalControlInput.x = 0.0;
		}
		else if (Math::abs(_totalControlInput.y) >= Math::abs(_movementInput.y))
		{
			_movementInput.y = 0.0f;
			_totalControlInput.y = 0.0;
		}
		else if (Math::abs(_totalControlInput.z) >= Math::abs(_movementInput.z))
		{
			_movementInput.z = 0.0f;
			_totalControlInput.z = 0.0;
		}*/

		//_movementInput = Vector3f::zero();

		/*ActorMovementComponent* c = getMovementComponent();
		if (c)
		{
			return c->resetMovementInput();
		}*/

		return defaultResetMovementInput();
	}

	InputComponent* ControllableActor::createInputComponent() 
	{
		AE_RELEASE(_inputComponent);

		_inputComponent = new InputComponent();

		return _inputComponent;
	}

	ActorMovementComponent* ControllableActor::getMovementComponent() const
	{
		return getComponent<ActorMovementComponent>();
	}

	ActorController* ControllableActor::getController() const
	{
		return _controller;
	}

	void ControllableActor::setController(ActorController* controller)
	{
		//ActorController* prevController = _controller;

		_controller = controller;
		//force update???

		//PLAYER STATE???

		//other stuff

		//receive event???
	}

	void ControllableActor::setRotation(Quaterniond rotation, double elapsedTime)
	{
		getLocalTransform().setRotation(rotation);//elapsedTime??
	}

	void ControllableActor::defaultAddMovementInput(Vector3f input)
	{
		if (isInputEnabled()) // || force
		{
			_movementInput += input;
		}
	}

	Vector3f ControllableActor::defaultGetCurrentMovementInput() const
	{
		return _movementInput;
	}

	Vector3f ControllableActor::defaultGetLastMovementInput() const
	{
		return _lastMovementInput;
	}

	Vector3f ControllableActor::defaultResetMovementInput()
	{
		_lastMovementInput = _movementInput;
		_movementInput = Vector3f::zero();
		return _lastMovementInput;
	}
}
