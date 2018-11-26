#include "Input.h"

#include "Controller.h"
#include "InputContext.h"

namespace Arcana
{
	Input& Input::instance()
	{
		static Input input;
		return input;
	}

	Input::Input() : _absolutePosition(Vector2i::zero()), _relativePosition(Vector2f::zero()), _filterMouse(true)
	{

	}

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

	void Input::setMouseSmoothingEnabled(bool smoothing)
	{
		_filterMouse = smoothing;
	}

	bool Input::isMouseSmoothingEnabled() const
	{
		return _filterMouse;
	}

	Vector2i Input::getMousePosition() const
	{
		return _absolutePosition;
	}

	Vector2i Input::getMousePosition(const Window& relativeWindow) const
	{
		WindowHandle handle = relativeWindow.getWindowHandle();
		return InputContext::getMousePosition(handle, _absolutePosition);
	}

	void Input::setMousePosition(const Vector2i& position)
	{
		InputContext::setMousePosition(position);

		_absolutePosition = position;
		_relativePosition = Vector2f::zero();
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
				_absolutePosition = position;
				_relativePosition = Vector2f::zero();
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

	Vector2f Input::getRelativeMousePosition() const
	{
		return _relativePosition;
	}

	float Input::getMouseWheelPosition() const
	{
		return 0.0f;
	}
}