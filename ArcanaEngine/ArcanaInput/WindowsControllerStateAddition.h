#ifndef WINDOWS_CONTROLLER_STATE_ADDITION_H_
#define WINDOWS_CONTROLLER_STATE_ADDITION_H_

#include "InputDefines.h"

#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")

namespace Arcana
{
	struct ARCANA_INPUT_API ControllerStateAddition
	{
		
		XINPUT_STATE xinputState;
	};
}

#endif
