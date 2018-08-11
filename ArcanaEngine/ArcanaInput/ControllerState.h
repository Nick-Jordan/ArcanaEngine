#ifndef CONTROLLER_STATE_H_
#define CONTROLLER_STATE_H_

#include "InputDefines.h"

#include "Key.h"
#include "ControllerStateAddition.h"
#include "Controller.h"

namespace Arcana
{

	class ARCANA_INPUT_API ControllerState
	{
	public:

		ControllerState()
		{
			connected = false;
			std::fill(floatAxes, floatAxes + Controller::FloatAxisCount, 0.0f);
			std::fill(buttons, buttons + Controller::ButtonCount, false);
		}
 
		bool connected;
		float floatAxes[Controller::FloatAxisCount];
		bool buttons[Controller::ButtonCount];

		ControllerStateAddition data;
	};

}

#endif // !CONTROLLER_STATE_H_

