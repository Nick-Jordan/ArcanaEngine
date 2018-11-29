#include "Input.h"

#include "Controller.h"
#include "InputContext.h"

namespace Arcana
{
	const Window* Input::__staticRelativeWindow = nullptr;

	bool Input::isKeyPressed(Key key)
	{
		if (key.isControllerKey())
		{
			for (uint32 i = 0; i < Controller::MaxControllers; i++)
			{
				if (isControllerConnected(i))
				{
					return Controller::isButtonPressed(i, key);
				}
			}
		}

		return InputContext::isKeyPressed(key);
	}

	float Input::getFloatAxis(Key key)
	{
		if(key.isFloatAxis())
		{
			if(key.isControllerKey())
			{
				for(uint32 i = 0; i < Controller::MaxControllers; i++)
				{
					if(isControllerConnected(i))
					{
						return getControllerFloatAxis(i, key);
					}
				}
			}

			if(key.getKeyCode() == KeyCode::MouseX)
			{
				return (float) getMousePosition().x;
			}
			else if(key.getKeyCode() == KeyCode::MouseY)
			{
				return (float) getMousePosition().y;
			}
			else if(key.getKeyCode() == KeyCode::MouseWheel)
			{
				return getMouseWheelPosition();
			}
		}	
		return 0.0f;
	}

	Vector2f Input::getVectorAxis(Key key)
	{
		return Vector2f::zero();
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
		if (__staticRelativeWindow)
		{
			return getMousePosition(*__staticRelativeWindow);
		}

		return InputContext::getMousePosition();
	}

	Vector2i Input::getMousePosition(const Window& relativeWindow)
	{
		WindowHandle handle = relativeWindow.getWindowHandle();
		return InputContext::getMousePosition(handle, InputContext::getMousePosition());
	}

	void Input::setMousePosition(const Vector2i& position)
	{
		if (__staticRelativeWindow)
		{
			setMousePosition(position, *__staticRelativeWindow);
		}
		else
		{
			InputContext::setMousePosition(position);
		}
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

	void Input::setStaticRelativeWindow(const Window& window)
	{
		__staticRelativeWindow = &window;
	}
}