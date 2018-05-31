#include "KeyListener.h"

#include "KeyEvent.h"

#include <iostream>

namespace Arcana
{
	KeyListener::KeyListener()
	{
		listenForEvent(1);
	}
		
	KeyListener::~KeyListener()
	{
	}
		
	bool KeyListener::processEvent(Event& event, EventHandler& handler)
	{
		Event::Data& data = event.getData();
		
		std::cout << "KeyEvent Type: " << (data["event"]._int ? "Released" : "Pressed") << std::endl;
		std::cout << "KeyEvent keyCode: " << data["keyCode"]._int << std::endl;
		std::cout << "KeyEvent alt: " << std::to_string(data["alt"]._bool) << std::endl;
		std::cout << "KeyEvent control: " << std::to_string(data["control"]._bool) << std::endl;
		std::cout << "KeyEvent shift: " << std::to_string(data["shift"]._bool) << std::endl;
		std::cout << "KeyEvent system: " << std::to_string(data["system"]._bool) << std::endl;
		
		return true;
	}
}