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
		KeyEvent::Data data = event.getData<KeyEvent::Data>();
		
		std::cout << "KeyEvent Type: " << (data.event ? "Released" : "Pressed") << std::endl;
		std::cout << "KeyEvent keyCode: " << data.keyCode << std::endl;
		std::cout << "KeyEvent alt: " << std::to_string(data.alt) << std::endl;
		std::cout << "KeyEvent control: " << std::to_string(data.control) << std::endl;
		std::cout << "KeyEvent shift: " << std::to_string(data.shift) << std::endl;
		std::cout << "KeyEvent system: " << std::to_string(data.system) << std::endl;
		
		return true;
	}
}