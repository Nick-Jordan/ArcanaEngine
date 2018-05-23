#include "EventHandler.h"

#include <iostream>

namespace Arcana
{
	//INITIALIZE_CATEGORY(Arcana, EventHandling)
	
	
	EventHandler::EventHandler() : Object("EventHandler")
	{
	}
	
	EventHandler::~EventHandler()
	{
	}
	
	EventHandler::BroadcastResult EventHandler::broadcast(Event& event)
	{
		
		bool processResults = true;

		std::cout << "Violation? 5" << std::endl;
		
		for(auto iter = _listeners.createIterator(); iter; ++iter)
		{
			if((*iter)->isListeningForEvent(event.getEventId()))
			{
				std::cout << "Process event" << std::endl;
				processResults &= (*iter)->processEvent(event, *this);
				event.getEventCallback().executeIfBound();
			}
		}

		std::cout << "Violation? 4" << std::endl;
		
		return processResults ? EVENT_SUCCESS : EVENT_ERROR;
	}
		
	void EventHandler::addEventListener(EventListener* ptr)
	{
		_listeners.add(ptr);
	}
		
	int32 EventHandler::removeEventListener(EventListener* ptr)
	{
		return _listeners.remove(ptr);
	}
}