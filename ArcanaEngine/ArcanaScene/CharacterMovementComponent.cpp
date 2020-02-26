#include "CharacterMovementComponent.h"

namespace Arcana
{
	CharacterMovementComponent::CharacterMovementComponent() : ActorMovementComponent()
	{
		/*_gravityScale = 1.f;
		_groundFriction = 8.0f;
		_jumpZVelocity = 420.0f;
		_jumpOffZFactor = 0.5f;
		_rotationRate = Quaterniond();// (0.f, 360.0f, 0.0f);
		//SetWalkableFloorZ(0.71f);

		_maxStepHeight = 45.0f;
		_perchRadiusThreshold = 0.0f;
		_perchAdditionalHeight = 40.f;

		_maxFlySpeed = 600.0f;
		_maxWalkSpeed = 600.0f;
		_maxSwimSpeed = 300.0f;
		_maxCustomMovementSpeed = _maxWalkSpeed;

		//MaxSimulationTimeStep = 0.05f;
		//MaxSimulationIterations = 8;

		NetworkSimulatedSmoothLocationTime = 0.100f;
		NetworkSimulatedSmoothRotationTime = 0.033f;

		_maxWalkSpeedCrouched = _maxWalkSpeed * 0.5f;
		_maxOutOfWaterStepHeight = 40.0f;
		_outofWaterZ = 420.0f;
		_airControl = 0.05f;
		_airControlBoostMultiplier = 2.f;
		_airControlBoostVelocityThreshold = 25.f;
		_fallingLateralFriction = 0.f;
		_maxAcceleration = 2048.0f;
		_brakingDecelerationWalking = _maxAcceleration;
		_brakingDecelerationFalling = 0.f;
		_brakingDecelerationFlying = 0.f;
		_brakingDecelerationSwimming = 0.f;
		_ledgeCheckThreshold = 4.0f;
		_jumpOutOfWaterPitch = 11.25f;

		_mass = 100.0f;
		_justTeleported = true;
		_crouchedHalfHeight = 40.0f;
		_buoyancy = 1.0f;
		_pendingLaunchVelocity = FVector::ZeroVector;
		_defaultWaterMovementMode = Swimming;
		_defaultLandMovementMode = Walking;
		_groundMovementMode = Walking;
		_forceNextFloorCheck = true;
		bShrinkProxyCapsule = true;
		bCanWalkOffLedges = true;
		bCanWalkOffLedgesWhenCrouching = false;
		bWantsToLeaveNavWalking = false;

		bEnablePhysicsInteraction = true;
		StandingDownwardForceScale = 1.0f;
		InitialPushForceFactor = 500.0f;
		PushForceFactor = 750000.0f;
		PushForcePointZOffsetFactor = -0.75f;
		bPushForceScaledToMass = false;
		bScalePushForceToVelocity = true;

		TouchForceFactor = 1.0f;
		bTouchForceScaledToMass = true;
		MinTouchForce = -1.0f;
		MaxTouchForce = 250.0f;
		RepulsionForce = 2.5f;

		bUseControllerDesiredRotation = false;

		bUseSeparateBrakingFriction = false; // Old default behavior.

		bMaintainHorizontalGroundVelocity = true;
		bImpartBaseVelocityX = true;
		bImpartBaseVelocityY = true;
		bImpartBaseVelocityZ = true;
		bImpartBaseAngularVelocity = true;
		bAlwaysCheckFloor = true;

		// default character can jump, walk, and swim
		NavAgentProps.bCanJump = true;
		NavAgentProps.bCanWalk = true;
		NavAgentProps.bCanSwim = true;
		ResetMoveState();

		ClientPredictionData = NULL;
		ServerPredictionData = NULL;

		// This should be greater than tolerated player timeout * 2.
		MinTimeBetweenTimeStampResets = 4.f * 60.f;

		bEnableScopedMovementUpdates = true;

		bRequestedMoveUseAcceleration = true;
		bUseRVOAvoidance = false;
		bUseRVOPostProcess = false;
		AvoidanceLockVelocity = FVector::ZeroVector;
		AvoidanceLockTimer = 0.0f;
		AvoidanceGroup.bGroup0 = true;
		GroupsToAvoid.Packed = 0xFFFFFFFF;
		GroupsToIgnore.Packed = 0;
		AvoidanceConsiderationRadius = 500.0f;

		OldBaseQuat = FQuat::Identity;
		OldBaseLocation = FVector::ZeroVector;

		NavMeshProjectionInterval = 0.1f;
		NavMeshProjectionInterpSpeed = 12.f;
		NavMeshProjectionHeightScaleUp = 0.67f;
		NavMeshProjectionHeightScaleDown = 1.0f;*/
	}
}