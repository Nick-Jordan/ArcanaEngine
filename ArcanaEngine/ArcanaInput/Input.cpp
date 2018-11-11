#include "Input.h"

#include "Controller.h"

namespace Arcana
{
	bool Input::isControllerConnected(uint32 controllerId)
	{
		return Controller::isConnected(controllerId);
	}

	bool Input::isControllerButtonPressed(uint32 controllerId, Key button)
	{
		return Controller::isButtonPressed(controllerId, button);
	}

	float Input::getControllerFloatAxis(uint32 controllerId, Key axis)
	{
		return Controller::getFloatAxis(controllerId, axis);
	}

	Vector2f Input::getControllerVectorAxis(uint32 controllerId, Key axis)
	{
		return Controller::getVectorAxis(controllerId, axis);
	}

	void Input::update()
	{
		Controller::update();
	}
}
