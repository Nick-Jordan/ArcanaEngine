#ifndef CONTROLLER_STATE_H_
#define CONTROLLER_STATE_H_

#include "InputDefines.h"

#include "Key.h"

namespace Arcana
{

	class ARCANA_INPUT_API ControllerState
	{
	public:

		ControllerState() {};

		virtual ~ControllerState() {};

		int32 getControllerId() { return 0; };


		virtual bool isConnected() = 0;

		virtual bool getButtonDown(Key key) = 0;

		virtual double getTriggerValue(Key key) = 0;
 
	};

}

#endif // !CONTROLLER_STATE_H_

