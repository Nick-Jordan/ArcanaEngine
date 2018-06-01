#ifndef EVENT_LISTENER_H_
#define EVENT_LISTENER_H_

#include "CoreDefines.h"

#include "Array.h"
#include "Types.h"

#include "Event.h"

namespace Arcana
{
	class ARCANA_CORE_API EventHandler;

	class ARCANA_CORE_API EventListener
	{
	public:
	
		virtual ~EventListener() {};
		
		virtual bool processEvent(Event& event, EventHandler& handler) = 0;
		
		
		void listenForEvent(uint64 eventId)
		{
			_eventIds.add(eventId);
		}
		
		bool isListeningForEvent(uint64 eventId) const
		{
			return _eventIds.contains(eventId);
		}
		
	private:
	
		Array<uint64> _eventIds;
	};
}

#endif // !EVENT_LISTENER_H_