#include "Input.h"

#include "Controller.h"
#include "InputContext.h"

namespace Arcana
{
	Vector2i Input::__absolutePosition = Vector2i::zero();

	bool Input::isKeyPressed(Key key)
	{
		return InputContext::isKeyPressed(key);
	}

	bool Input::isControllerConnected(uint32 controllerId)
	{
		return Controller::isConnected(controllerId);
	}

	float Input::getControllerFloatAxis(uint32 controllerId, Key key)
	{
		return Controller::getFloatAxis(controllerId, key);
	}

	Vector2f Input::getControllerVectorAxis(uint32 controllerId, Key axis)
	{
		return Controller::getVectorAxis(controllerId, axis);
	}

	Vector2i Input::getMousePosition()
	{
		__absolutePosition = InputContext::getMousePosition();
		return __absolutePosition;
	}

	Vector2i Input::getMousePosition(const Window& relativeWindow)
	{
		WindowHandle handle = relativeWindow.getWindowHandle();
		return InputContext::getMousePosition(handle, __absolutePosition);
	}

	void Input::setMousePosition(const Vector2i& position)
	{
		InputContext::setMousePosition(position);

		__absolutePosition = position;
	}

	void Input::setMousePosition(const Vector2i& position, const Window& relativeWindow)
	{
		WindowHandle handle = relativeWindow.getWindowHandle();
		if (handle)
		{
			POINT pt = { position.x, position.y };

			if (ClientToScreen(handle, &pt))
			{
				SetCursorPos(pt.x, pt.y);
				__absolutePosition = position;
			}
			else
			{
				setMousePosition(position);
			}
		}
		else
		{
			setMousePosition(position);
		}
	}

	float Input::getMouseWheelPosition()
	{
		return 0.0f;
	}
}