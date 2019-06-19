#include "PlayerController.h"

#undef min

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

		_rotationSpeed = 10.0;
		_rotationSensitivity = 0.05;
		_rotationRollSensitivity = 1.0;
	}

	void PlayerController::update(double elapsedTime)
	{
		updateDesiredRotation();
		updateRotation(elapsedTime);
	}

	void PlayerController::destroyed()
	{

	}

	void PlayerController::setControllerRotation(const Quaterniond& rotation)
	{
		_desiredRotation = rotation;
	}

	const Quaterniond& PlayerController::getDesiredRotation() const
	{
		return _desiredRotation;
	}

	bool PlayerController::isPlayerController() const
	{
		return true;
	}


	double PlayerController::getRotationSpeed() const
	{
		return _rotationSpeed;
	}

	void PlayerController::setRotationSpeed(double rotationSpeed)
	{
		_rotationSpeed = rotationSpeed;
	}

	double PlayerController::getRotationSensitivity() const
	{
		return _rotationSensitivity;
	}

	void PlayerController::setRotationSensitivity(double rotationSensitivity)
	{
		_rotationSensitivity = rotationSensitivity;
	}

	double PlayerController::getRotationRollSensitivity() const
	{
		return _rotationRollSensitivity;
	}

	void PlayerController::setRotationRollSensitivity(double rotationRollSensitivity)
	{
		_rotationRollSensitivity = rotationRollSensitivity;
	}

	void PlayerController::addPitchInput(float input)
	{
		_tempRotation.x = input * _rotationSensitivity;
	}

	void PlayerController::addRollInput(float input)
	{
		_tempRotation.z = input * _rotationRollSensitivity;
	}

	void PlayerController::addYawInput(float input)
	{
		_tempRotation.y = input * _rotationSensitivity;
	}

	void PlayerController::updateRotation(double elapsedTime)
	{

		Quaterniond current = getControllerRotation();

		current.normalize();
		_desiredRotation.normalize();

		double d = 1.0 - (current.x * _desiredRotation.x + current.y * _desiredRotation.y + current.z * _desiredRotation.z + current.w * _desiredRotation.w)
			* (current.x * _desiredRotation.x + current.y * _desiredRotation.y + current.z * _desiredRotation.z + current.w * _desiredRotation.w);

		if (abs(d) < Math::EPSILON)
		{
			_tempRotation = Vector3f::zero();
			return;
		}

		double rotationDistance = elapsedTime * _rotationSpeed;

		ActorController::setControllerRotation(Quaterniond::slerp(current, _desiredRotation, Math::clamp(rotationDistance, 0.0, 1.0)));

		//LOGF(Info, CoreEngine, "distance: %f --- rotationDistance: %f", d, rotationDistance);

		//reset input
		Quaterniond newRotation = getControllerRotation();

		if (abs(newRotation.x - _desiredRotation.x) < Math::EPSILON
			&& abs(newRotation.y - _desiredRotation.y) < Math::EPSILON
			&& abs(newRotation.z - _desiredRotation.z) < Math::EPSILON
			&& abs(newRotation.w - _desiredRotation.w) < Math::EPSILON)
		{
			_tempRotation = Vector3f::zero();
		}
	}

	void PlayerController::updateDesiredRotation()
	{

		float yRotation = _tempRotation.y;
		float xRotation = _tempRotation.x;
		float zRotation = _tempRotation.z;

		if (abs(xRotation) > 0.02 || abs(yRotation) > 0.02 || abs(zRotation) > 0.02)
		{
			xRotation = abs(xRotation) < 0.02 ? 0.0f : xRotation;
			yRotation = abs(yRotation) < 0.02 ? 0.0f : yRotation;
			zRotation = abs(zRotation) < 0.02 ? 0.0f : zRotation;

			Quaterniond quatY;
			quatY.fromAxisAngle(Vector3d::unitY(), -yRotation);

			Quaterniond quatX;
			quatX.fromAxisAngle(Vector3d::unitX(), xRotation);

			Quaterniond quatZ;
			quatZ.fromAxisAngle(Vector3d::unitZ(), zRotation);

			_desiredRotation *= quatY * quatX * quatZ;

			_desiredRotation.normalize();
		}
	}
}