#ifndef XBOX_CONTROLLER_H_
#define XBOX_CONTROLLER_H_

#include "InputDefines.h"

#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")

namespace Arcana
{

	class ARCANA_INPUT_API XBOXController
	{
	public:

		XBOXController();
		~XBOXController();

		XINPUT_STATE getState();
		bool isConnected();
		void vibrate(int leftVal = 0, int rightVal = 0);

	private:

		XINPUT_STATE _controllerState;
		int _controllerNum;
	};

}

#endif // !XBOX_CONTROLLER_H_

