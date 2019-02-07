#ifndef NO_DATA_EVENTS_H_
#define NO_DATA_EVENTS_H_

#include "Event.h"

namespace Arcana
{
	/** \brief Event broadcasted on window close.
	 *
	 *  WindowClosedEvent have and event id of 3.
	 */
	class WindowClosedEvent : public Event
	{
	public:

		/** \brief WindowClosedEvent default constructor.
		 */
		WindowClosedEvent() : Event(EventID::WindowClosedEventID) {};

		/** \brief WindowClosedEvent destructor.
		 */
		~WindowClosedEvent() {};
	};

}

#endif // !NO_DATA_EVENTS_H_
