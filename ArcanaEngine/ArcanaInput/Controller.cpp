#include "Controller.h"

#include "ControllerManager.h"
#include "ControllerState.h"

namespace Arcana
{

	bool Controller::isConnected(uint32 controllerId)
	{
		return ControllerManager::instance().getState(controllerId).connected;
	}

	bool Controller::isButtonPressed(uint32 controllerId, Key button)
	{
		if (!button.isControllerKey())
		{
			return false;
		}

		return ControllerManager::instance().getState(controllerId).buttons[StateButtonMapping(button.getKeyCode())];
	}

	float Controller::getFloatAxis(uint32 controllerId, Key axis)
	{
		if (!axis.isControllerKey() || !axis.isFloatAxis())
		{
			return 0.0f;
		}

		return ControllerManager::instance().getState(controllerId).floatAxes[StateAxisMapping(axis.getKeyCode())];
	}

	Vector2f Controller::getVectorAxis(uint32 controllerId, Key axis)
	{
		return Vector2f::zero();
	}

	Controller::Specification Controller::getSpecification(uint32 controllerId)
	{
		return ControllerManager::instance().getSpecification(controllerId);
	}

	void Controller::update()
	{
		return ControllerManager::instance().update();
	}

	Controller::Specification::Specification() :
		name("Unknown Controller"),
		manufacturer(0),
		productId(0)
	{

	}

}
