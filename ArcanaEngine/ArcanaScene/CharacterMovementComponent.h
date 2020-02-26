#ifndef CHARACTER_MOVEMENT_COMPONENT_H_
#define CHARACTER_MOVEMENT_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorMovementComponent.h"
#include "Character.h"

namespace Arcana
{
	enum MovementMode
	{
		UnknownMovementMode,
		Walking,
		Falling,
		Swimming,
		Flying,
		Custom,
		NumMovementModes
	};

	class ARCANA_SCENE_API CharacterMovementComponent : public ActorMovementComponent
	{
	public:

		CharacterMovementComponent();

	private:

		Character* _owner;

		float _gravityScale;
		float _maxStepHeight;
		float _jumpZVelocity;
		float _jumpOffZFactor;

		float _walkableFloorAngle;
		//floor z?

		MovementMode _mode;
		//custom movement mode
		//base rotation and position

		float _groundFriction;
		float _maxWalkSpeed;
		float _maxWalkSpeedCrouched;
		float _maxSwimSpeed;
		float _maxFlySpeed;
		float _maxCustomMovementSpeed;
		float _maxAcceleration;
		float _brakingFriction;
		float _brakingDecelerationWalking;
		float _brakingDecelerationFalling;
		float _brakingDecelerationSwimming;
		float _brakingDecelerationFlying;
		float _airControl;
		float _airControlBoostMultiplier;
		float _airControlBoostVelocityThreshold;
		float _fallingLateralFriction;
		float _crouchedHalfHeight;
		float _buoyancy;
		float _perchRadiusThreshold;
		float _perchAdditionalHeight;

		Quaterniond _rotationRate;
		bool _useControllerDesiredRotation;
		bool _orientRotationToMovement;
		bool _useSeparateBrakingFriction;
	};
}

#endif // !CHARACTER_MOVEMENT_COMPONENT_H_