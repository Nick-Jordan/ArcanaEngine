#include "FPSCharacter.h"

namespace Arcana
{

	FPSCharacter::FPSCharacter()
	{
	}


	FPSCharacter::~FPSCharacter()
	{
	}

	void FPSCharacter::begin()
	{
		Character::begin();

	}

	void FPSCharacter::setupInputComponent(InputComponent* inputComponent)
	{
		inputComponent->bindAxisMapping("MoveForwards", this, &FPSCharacter::moveForward);
		inputComponent->bindAxisMapping("MoveRight", this, &FPSCharacter::moveRight);
		inputComponent->bindAxisMapping("MoveUp", this, &FPSCharacter::moveUp);

		inputComponent->bindAxisMapping("LookHorizontal", this, &FPSCharacter::lookHorizontal);
		inputComponent->bindAxisMapping("LookVertical", this, &FPSCharacter::lookVertical);
		inputComponent->bindAxisMapping("Roll", this, &FPSCharacter::roll);

		inputComponent->bindActionMapping("Jump", KeyEvent::Pressed, false, this, &FPSCharacter::startJump);
		inputComponent->bindActionMapping("Jump", KeyEvent::Released, false, this, &FPSCharacter::stopJump);
	}

	void FPSCharacter::moveForward(float input)
	{
		if (input != 0.0f && getController())
		{
			Vector3f dir = getSceneComponent()->getForwardVector().cast<float>();

			addMovementInput(dir, input);
		}
	}

	void FPSCharacter::moveRight(float input)
	{
		if (input != 0.0f && getController())
		{
			Vector3f dir = getSceneComponent()->getRightVector().cast<float>();

			addMovementInput(dir, input);
		}

	}

	void FPSCharacter::moveUp(float input)
	{
		if (input != 0.0f && getController())
		{
			Vector3f dir = getSceneComponent()->getUpVector().cast<float>();

			addMovementInput(dir, input);
		}
	}

	void FPSCharacter::lookHorizontal(float input)
	{
		Character::addYawInput(input);
	}

	void FPSCharacter::lookVertical(float input)
	{
		Character::addPitchInput(input);
	}

	void FPSCharacter::roll(float input)
	{
		Character::addRollInput(input);
	}

	void FPSCharacter::startJump()
	{
		LOG(Info, CoreEngine, "started jump");
	}

	void FPSCharacter::stopJump()
	{
		LOG(Info, CoreEngine, "stopped jump");
	}
}
