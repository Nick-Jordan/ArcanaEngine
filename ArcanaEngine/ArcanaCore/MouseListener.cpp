#include "MouseListener.h"

#include "MouseEvent.h"

#include <iostream>

namespace Arcana
{
	MouseListener::MouseListener()
	{
		listenForEvent(2);
	}
		
	MouseListener::~MouseListener()
	{
	}
		
	bool MouseListener::processEvent(Event& event, EventHandler& handler)
	{
		MouseEvent::Data data = event.getData<MouseEvent::Data>();
		
		std::cout << "MouseEvent Type: " << data.event << std::endl;
		std::cout << "MouseEvent x: " << data.x << std::endl;
		std::cout << "MouseEvent y: " << data.y << std::endl;
		
		return true;
	}
}