#include "PlayerController.h"

namespace Arcana
{
	PlayerController::PlayerController() : ActorController()
	{

	}

	PlayerController::PlayerController(const std::string& id) : ActorController(id)
	{
	}


	PlayerController::~PlayerController()
	{
	}

	void PlayerController::initialize(std::string name, const Actor* templateActor)
	{
		ActorController::initialize(name, templateActor);

		_pitchScale = 1.0;
		_yawScale = 1.0;
		_rollScale = 1.0;
	}

	void PlayerController::update(double elapsedTime)
	{
		ActorController::update(elapsedTime);

		updateRotation(elapsedTime);//??????????????? keep???????????????
	}

	bool PlayerController::isLocalPlayerController() const
	{
		return true;
	}

	void PlayerController::addPitchInput(float input)
	{
		Quaterniond quat;
		quat.fromAxisAngle(Vector3d::unitX(), input * _pitchScale);
		_rotationPitchInput = quat;
	}

	void PlayerController::addYawInput(float input)
	{
		Quaterniond quat;
		quat.fromAxisAngle(Vector3d::unitY(), input * _yawScale);
		_rotationYawInput = quat;
	}

	void PlayerController::addRollInput(float input)
	{
		Quaterniond quat;
		quat.fromAxisAngle(Vector3d::unitZ(), input * _rollScale);
		_rotationRollInput = quat;
	}

	void PlayerController::updateRotation(double elapsedTime)
	{
		Quaterniond viewRotation = getControllerRotation();

		ControllableActor* ca = getControllingActor();
		if (ca)
		{
			ca->getLocalTransform().rotate(_rotationPitchInput);
			ca->getLocalTransform().rotate(_rotationYawInput);
			ca->getLocalTransform().rotate(_rotationRollInput);
		}

		_rotationPitchInput = _rotationYawInput = _rotationRollInput = Quaterniond::IDENTITY;

		/*Quaternionf input = _rotationPitchInput * _rotationYawInput * _rotationRollInput;

		//player camera manager processing
		viewRotation *= input.cast<double>();
		_rotationPitchInput = _rotationYawInput = _rotationRollInput = Quaternionf::IDENTITY;
		//player camera manager processing

		//other stuff

		setControllerRotation(viewRotation);

		ControllableActor* ca = getControllingActor();//spectator???
		if (ca)
		{
			ca->setRotation(viewRotation, elapsedTime);
		}*/
	}
}
