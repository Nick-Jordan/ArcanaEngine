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
		Event::Data data = event.getData();

		std::cout << "MouseEvent Type: " << data["event"]._int << std::endl;
		std::cout << "MouseEvent x: " << data["x"]._int << std::endl;
		std::cout << "MouseEvent y: " << data["y"]._int << std::endl;
		
		return true;
	}
}