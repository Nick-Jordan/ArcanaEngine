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

		PlayerController* c = getWorld()->createActor<PlayerController>("controller", new Transform());
		c->control(this);

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
		ControllableActor::addPitchInput(input);
	}

	void Character::addRollInput(float input)
	{
		ControllableActor::addRollInput(input);
	}

	void Character::addYawInput(float input)
	{
		ControllableActor::addYawInput(input);
	}
}
