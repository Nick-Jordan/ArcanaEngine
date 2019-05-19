#include "Player.h"

namespace Arcana
{

	Player::Player() : _controller(nullptr)
	{
	}


	Player::~Player()
	{
		AE_RELEASE(_controller);
	}

	bool Player::execute(World* world, const std::string& command)
	{
		return false;
	}

	PlayerController* Player::getController(const World* world) const
	{
		return nullptr;
	}

	void Player::setController(PlayerController* controller)
	{
		/*if (_controller)
		{
			_controller->_player = nullptr;
		}

		AE_RELEASE(_controller);

		if (controller)
		{
			controller->_player = this;
		}

		_controller = controller;

		if (_controller)
		{
			_controller->reference();
		}*/
	}
}
