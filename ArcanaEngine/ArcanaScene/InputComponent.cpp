#include "InputComponent.h"

#include "Controller.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Arcana
{
	InputComponent::InputComponent()
	{
		listenForEvent(EventID::KeyEventID);
	}


	InputComponent::~InputComponent()
	{
	}

	


	bool InputComponent::processEvent(Event& event, EventHandler& handler)
	{
		if (event.getEventId() == EventID::KeyEventID)
		{
			int32 keyCode = event.getInt("keyCode");
		}
		else if (event.getEventId() == EventID::MouseEventID)
		{
		
		}

		return true;
	}

	void InputComponent::initialize()
	{

	}

	void InputComponent::update(double elapsedTime)
	{
		
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
