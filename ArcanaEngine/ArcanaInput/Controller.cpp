#include "Controller.h"


//xbox windows
#include "XInputState.h"

namespace Arcana
{

	Controller::Controller()
	{
		initialize();
	}


	Controller::~Controller()
	{
	}

	void Controller::initialize()
	{
		_state = new XInputState();
	}

	bool Controller::isConnected()
	{
		if (!_state)
			return false;

		return _state->isConnected();
	}

	ControllerState* Controller::getState()
	{
		return _state;
	}

}
