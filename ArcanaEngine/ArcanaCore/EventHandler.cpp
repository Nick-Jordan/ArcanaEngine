#include "EventHandler.h"

#include "ArcanaLog.h"

#include "ArcanaTemplate.h"

#include <iostream>

namespace Arcana
{
	
	
	EventHandler::EventHandler() : Object("EventHandler")
	{
	}
	
	EventHandler::~EventHandler()
	{
	}
	
	EventHandler::BroadcastResult EventHandler::broadcast(Event& event)
	{
		//test();

		bool processResults = true;
		
		event.getEventCallback().executeIfBound();

		for(auto iter = _listeners.createIterator(); iter; ++iter)
		{

			//SmartPtr<EventListener>& ptr = (*iter);
			std::shared_ptr<EventListener>& ptr = (*iter);

			if((*iter)->isListeningForEvent(event.getEventId()))
			{
				processResults &= (*iter)->processEvent(event, *this);
			}
		}
		
		return processResults ? EVENT_SUCCESS : EVENT_ERROR;
	}
		
	/*void EventHandler::addEventListener(SmartPtr<EventListener> ptr)
	{
		LOGF(Error, CoreEngine, "%d", ptr.get()->test);
		LOGF(Error, CoreEngine, "%p", ptr.get());
		_listeners.add(ptr);

		LOGF(Error, CoreEngine, "%d", _listeners[0].get()->test);
		LOGF(Error, CoreEngine, "%p", _listeners[0].get());
		test();
	}*/

	void EventHandler::addEventListener(std::shared_ptr<EventListener> ptr)
	{
		_listeners.add(ptr);
	}
		
	/*int32 EventHandler::removeEventListener(SmartPtr<EventListener> ptr)
	{
		return _listeners.remove(ptr);
	}*/
}