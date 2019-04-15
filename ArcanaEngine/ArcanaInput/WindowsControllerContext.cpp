#include "WindowsControllerContext.h"

#include "ControllerManager.h"

#include "ArcanaLog.h"

namespace Arcana
{
	bool ControllerContext::lazyUpdates = false;


	void ControllerContext::initialize()
	{
		updateConnections();
	}

	void ControllerContext::cleanup()
	{

	}

	bool ControllerContext::isConnected(uint32 index)
	{
		ControllerState state;

		ZeroMemory(&state.data.xinputState, sizeof(XINPUT_STATE));

		DWORD Result = XInputGetState(index, &state.data.xinputState);

		return Result == ERROR_SUCCESS;
	}

	void ControllerContext::setLazyUpdates(bool status)
	{
		lazyUpdates = status;
	}

	void ControllerContext::updateConnections()
	{
		for (uint32 i = 0; i < Controller::MaxControllers; i++)
		{
			LOGF(Error, CoreEngine, "Checking connection for controller %d...", i);
			bool connected = isConnected(i);
			ControllerManager::instance()._controllers[i].state.connected = connected;
			LOGF(Error, CoreEngine, "Controller %d %s connected..", i, connected ? "is" : "is not");			
		}
	}

	bool ControllerContext::open(uint32 index)
	{
		return false;
	}

	void ControllerContext::close()
	{

	}

	Controller::Specification ControllerContext::getSpecification() const
	{
		return _specification;
	}

	ControllerState ControllerContext::update()
	{
		ControllerState state;

		ZeroMemory(&state.data.xinputState, sizeof(XINPUT_STATE));
		DWORD Result = XInputGetState(_index, &state.data.xinputState);
		if (Result == ERROR_SUCCESS)
		{
			state.connected = true;

			for (int32 key = KeyCode::ControllerFaceButtonBottom; key < KeyCode::ControllerDPadRight; key++)
			{
				int32 index = StateButtonMapping(key);

				int32 xinputKey = keyToButtonConversion(key);

				state.buttons[index] = state.data.xinputState.Gamepad.wButtons & xinputKey;
			}

			state.floatAxes[0] = Math::range((float)state.data.xinputState.Gamepad.sThumbLY, -32768.0f, 32767.0f, -1.0f, 1.0f);
			state.floatAxes[1] = Math::range((float)state.data.xinputState.Gamepad.sThumbLX, -32768.0f, 32767.0f, -1.0f, 1.0f);
			state.floatAxes[2] = Math::range((float)state.data.xinputState.Gamepad.sThumbRY, -32768.0f, 32767.0f, -1.0f, 1.0f);
			state.floatAxes[3] = Math::range((float)state.data.xinputState.Gamepad.sThumbRX, -32768.0f, 32767.0f, -1.0f, 1.0f);
			state.floatAxes[4] = Math::range((float)state.data.xinputState.Gamepad.bLeftTrigger, 0.0f, 255.0f, 0.0f, 1.0f);
			state.floatAxes[5] = Math::range((float)state.data.xinputState.Gamepad.bRightTrigger, 0.0f, 255.0f, 0.0f, 1.0f);
		}

		return state;
	}

	int32 ControllerContext::keyToButtonConversion(int32 key)
	{
		switch (key)
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
}
