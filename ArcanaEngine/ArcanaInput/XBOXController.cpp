#include "XBOXController.h"

namespace Arcana
{

	XBOXController::XBOXController()
	{
		_controllerNum = 0;
	}


	XBOXController::~XBOXController()
	{
	}

	XINPUT_STATE XBOXController::getState()
	{
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		XInputGetState(_controllerNum, &_controllerState);

		return _controllerState;
	}

	bool XBOXController::isConnected()
	{
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		DWORD Result = XInputGetState(_controllerNum, &_controllerState);

		return Result == ERROR_SUCCESS;
	}

	void XBOXController::vibrate(int leftVal, int rightVal)
	{
		XINPUT_VIBRATION Vibration;

		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		XInputSetState(_controllerNum, &Vibration);
	}
}