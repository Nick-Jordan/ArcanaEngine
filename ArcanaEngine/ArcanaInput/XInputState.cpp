#include "XInputState.h"

#include "ArcanaLog.h"

namespace Arcana
{

	XInputState::XInputState()
	{
	}


	XInputState::~XInputState()
	{
	}

	bool XInputState::isConnected()
	{
		ZeroMemory(&_xinputState, sizeof(XINPUT_STATE));

		DWORD Result = XInputGetState(getControllerId(), &_xinputState);

		return Result == ERROR_SUCCESS;
	}

	bool XInputState::getButtonDown(Key key)
	{
		if (key.isControllerKey())
		{
			int32 xinputKey = keyToButtonConversion(key);

			return _xinputState.Gamepad.wButtons & xinputKey;
		}

		return false;
	}

	double XInputState::getTriggerValue(Key key)
	{
		if (key.isControllerKey())
		{

		}

		return 0.0;
	}

	int32 XInputState::keyToButtonConversion(Key key)
	{
		switch (key.getKeyCode())
		{
		case ControllerFaceButtonBottom:
			return XINPUT_GAMEPAD_A;
		case ControllerFaceButtonRight:
			return XINPUT_GAMEPAD_B;
		case ControllerFaceButtonLeft:
			return XINPUT_GAMEPAD_X;
		case ControllerFaceButtonTop:
			return XINPUT_GAMEPAD_Y;
		case ControllerLeftShoulder:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case ControllerRightShoulder:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case ControllerSpecialLeft:
			return XINPUT_GAMEPAD_BACK;
		case ControllerSpecialRight:
			return XINPUT_GAMEPAD_START;
		case ControllerLeftThumbstick:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case ControllerRightThumbstick:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case ControllerDPadUp:
			return XINPUT_GAMEPAD_DPAD_UP;
		case ControllerDPadDown:
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case ControllerDPadLeft:
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case ControllerDPadRight:
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		}

		return 0;
	}

	Key XInputState::virtualKeyConversion(WPARAM key, LPARAM flags)
	{
		switch (key)
		{
		case VK_PAD_A:
			return Keys::ControllerFaceButtonBottom;
		case VK_PAD_B:
			return Keys::ControllerFaceButtonRight;
		case VK_PAD_X:
			return Keys::ControllerFaceButtonLeft;
		case VK_PAD_Y:
			return Keys::ControllerFaceButtonTop;
		case VK_PAD_RSHOULDER:
			return Keys::ControllerRightShoulder;
		case VK_PAD_LSHOULDER:
			return Keys::ControllerLeftShoulder;
		case VK_PAD_LTRIGGER:
			return Keys::ControllerLeftTrigger;
		case VK_PAD_RTRIGGER:
			return Keys::ControllerRightTrigger;

		case VK_PAD_DPAD_UP:
			return Keys::ControllerDPadUp;
		case VK_PAD_DPAD_DOWN:
			return Keys::ControllerDPadDown;
		case VK_PAD_DPAD_LEFT:
			return Keys::ControllerDPadLeft;
		case VK_PAD_DPAD_RIGHT:
			return Keys::ControllerDPadRight;
		case VK_PAD_START:
			return Keys::ControllerSpecialRight;
		case VK_PAD_BACK:
			return Keys::ControllerSpecialLeft;
		case VK_PAD_LTHUMB_PRESS:
			return Keys::ControllerLeftThumbstick;
		case VK_PAD_RTHUMB_PRESS:
			return Keys::ControllerRightThumbstick;

		case VK_PAD_LTHUMB_UP:
			return Keys::ControllerLeftStickUp;
		case VK_PAD_LTHUMB_DOWN:
			return Keys::ControllerLeftStickDown;
		case VK_PAD_LTHUMB_RIGHT:
			return Keys::ControllerLeftStickRight;
		case VK_PAD_LTHUMB_LEFT:
			return Keys::ControllerLeftStickLeft;
		case VK_PAD_LTHUMB_UPLEFT:
			return Keys::ErrorKey;
		case VK_PAD_LTHUMB_UPRIGHT:
			return Keys::ErrorKey;
		case VK_PAD_LTHUMB_DOWNRIGHT:
			return Keys::ErrorKey;
		case VK_PAD_LTHUMB_DOWNLEFT:
			return Keys::ErrorKey;

		case VK_PAD_RTHUMB_UP:
			return Keys::ControllerRightStickUp;
		case VK_PAD_RTHUMB_DOWN:
			return Keys::ControllerRightStickDown;
		case VK_PAD_RTHUMB_RIGHT:
			return Keys::ControllerRightStickRight;
		case VK_PAD_RTHUMB_LEFT:
			return Keys::ControllerRightStickLeft;
		case VK_PAD_RTHUMB_UPLEFT:
			return Keys::ErrorKey;
		case VK_PAD_RTHUMB_UPRIGHT:
			return Keys::ErrorKey;
		case VK_PAD_RTHUMB_DOWNRIGHT:
			return Keys::ErrorKey;
		case VK_PAD_RTHUMB_DOWNLEFT:
			return Keys::ErrorKey;
		}

		return Keys::ErrorKey;
	}
}
