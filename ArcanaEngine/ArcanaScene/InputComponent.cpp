#include "InputComponent.h"

#include "Controller.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Arcana
{
	InputComponent::InputComponent() : _pressedKeys{ 0 }, _lastMouseX(-1), _lastMouseY(-1)
	{
		listenForEvent(EventID::KeyEventID);
	}


	InputComponent::~InputComponent()
	{
	}

	bool InputComponent::isKeyPressed(Key key)
	{
		return _pressedKeys[key.getKeyCode()];
	}

	int32 InputComponent::getMouseX() const
	{
		return _mouseX;
	}

	int32 InputComponent::getMouseY() const
	{
		return _mouseY;
	}

	bool InputComponent::isControllerConnected(uint32 controllerId)
	{
		return Controller::isConnected(controllerId);
	}

	float InputComponent::getControllerFloatAxis(uint32 controllerId, Key key)
	{
		return Controller::getFloatAxis(controllerId, key);
	}

	Vector2f InputComponent::getControllerVectorAxis(uint32 controllerId, Key axis)
	{
		return Controller::getVectorAxis(controllerId, axis);
	}

	MouseMovementCallback& InputComponent::getMouseMovementCallback()
	{
		return _mouseMovementCallback;
	}


	bool InputComponent::processEvent(Event& event, EventHandler& handler)
	{
		if (event.getEventId() == EventID::KeyEventID)
		{
			int32 keyCode = event.getInt("keyCode");

			if (event.getInt("event") == KeyEvent::Type::Pressed)
			{
				_pressedKeys[keyCode] = true;
			}
			else if (event.getInt("event") == KeyEvent::Type::Released)
			{
				_pressedKeys[keyCode] = false;
			}
		}
		else if (event.getEventId() == EventID::MouseEventID)
		{
			if (event.getInt("event") == MouseEvent::Type::Moved)
			{
				_mouseX = event.getInt("x");
				_mouseY = event.getInt("y");

				if (_lastMouseX == -1)
					_lastMouseX = _mouseX;
				if (_lastMouseY == -1)
					_lastMouseY == _mouseY;

				int32 deltaX = _mouseX - _lastMouseX;
				int32 deltaY = _mouseY - _lastMouseY;

				_lastMouseX = _mouseX;
				_lastMouseY = _mouseY;

				_mouseMovementCallback.executeIfBound(_mouseX, _mouseY, deltaX, deltaY);
			}
		}

		return true;
	}

	void InputComponent::initialize()
	{

	}

	void InputComponent::update(double elapsedTime)
	{
		Controller::update();
	}

	void InputComponent::registered()
	{

	}

	void InputComponent::unregistered()
	{

	}

	void InputComponent::componentDestroyed()
	{

	}
}
