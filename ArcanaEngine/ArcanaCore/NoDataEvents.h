#ifndef NO_DATA_EVENTS_H_
#define NO_DATA_EVENTS_H_

#include "Event.h"

namespace Arcana
{

	class WindowClosedEvent : public Event
	{
	public:

		WindowClosedEvent() : Event(EventID::WindowClosedEventID) {};

		~WindowClosedEvent() {};
	};

}

#endif // !NO_DATA_EVENTS_H_
