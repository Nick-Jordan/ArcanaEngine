#include "Character.h"

#include "World.h"
#include "PlayerController.h"

namespace Arcana
{

	Character::Character() : ControllableActor()
	{
	}


	Character::~Character()
	{
	}

	void Character::initialize(std::string name, const Actor* templateActor)
	{
		Actor::initialize(name, templateActor);
	}

	void Character::update(double elapsedTime)
	{
		ControllableActor::update(elapsedTime);
	}

	void Character::begin()
	{
		addComponent(new InputComponent());

		_playerController = getWorld()->createActor<PlayerController>("controller", new Transform());
		_playerController->control(this);

		if (_inputComponent)
		{
			setupInputComponent(_inputComponent);
		}
	}

	void Character::setupInputComponent(InputComponent* inputComponent)
	{

	}

	void Character::addPitchInput(float input)
	{
		_playerController->addPitchInput(input);
	}

	void Character::addRollInput(float input)
	{
		_playerController->addRollInput(input);
	}

	void Character::addYawInput(float input)
	{
		_playerController->addYawInput(input);
	}
}
