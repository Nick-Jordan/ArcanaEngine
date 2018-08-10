#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "InputDefines.h"

#include "Types.h"
#include "ControllerState.h"

namespace Arcana
{

	class ARCANA_INPUT_API Controller
	{
	public:

		Controller();

		~Controller();


		void initialize();

		bool isConnected();

		ControllerState* getState();

		//virtual void vibrate(int32 left, int32 right) = 0;

	private:

		ControllerState* _state;

	};

}

#endif // !CONTROLLER_H_

