#include "EventHandler.h"

namespace Arcana
{
	//INITIALIZE_CATEGORY(Arcana, EventHandling)
	
	
	EventHandler::EventHandler() : Object("EventHandler")
	{
	}
	
	EventHandler::~EventHandler()
	{
	}
	
	EventHandler::BroadcastResult EventHandler::broadcast(Event event)
	{
		
		bool processResults = true;
		
		for(auto iter = _listeners.createIterator(); iter; ++iter)
		{
			if((*iter)->isListeningForEvent(event.getEventId()))
			{
				processResults &= (*iter)->processEvent(event, *this);
			}
		}
		
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