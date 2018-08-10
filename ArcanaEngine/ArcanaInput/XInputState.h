#ifndef XINPUT_STATE_H_
#define XINPUT_STATE_H_

#include "InputDefines.h"

#include "ControllerState.h"

#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")


namespace Arcana
{

	class ARCANA_INPUT_API XInputState : public ControllerState
	{
	public:
		XInputState();
		~XInputState();

		virtual bool isConnected() override;

		virtual bool getButtonDown(Key key) override;

		virtual double getTriggerValue(Key key) override;

	private:

		static int32 keyToButtonConversion(Key key);
		//test
	public:
		static Key virtualKeyConversion(WPARAM key, LPARAM flags);

	private:
		XINPUT_STATE _xinputState;
	};

}

#endif // !XINPUT_STATE_H_

