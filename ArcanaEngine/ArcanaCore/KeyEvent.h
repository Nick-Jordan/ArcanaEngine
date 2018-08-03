#ifndef KEY_EVENT_H_
#define KEY_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"

namespace Arcana
{

	/** \brief Event broadcasted on key presses/releases.
	 *
	 *  KeyEvents have and event id of 1.
	 */

	class KeyEvent : public Event
	{
	public:
	
		/** \brief Enum defining the event type
		 */

		enum Type
		{
			Pressed,
			Released
		};
	
		/** \brief KeyEvent constructor.
		 */

		KeyEvent(Type event, int32 keyCode, bool alt, bool control, bool shift, bool system) : Event(1)
		{
			getData().addInt("event", event);
			getData().addInt("keyCode", keyCode);
			getData().addBool("alt", alt);
			getData().addBool("control", control);
			getData().addBool("shift", shift);
			getData().addBool("system", system);

			getEventCallback().bind(eventCallback);
		};

	private:

		/** \brief Test function for the KeyEvent callback.
		 */

		static void eventCallback()
		{
			LOG(Info, CoreEngine, "KeyEvent broadcasted.");
		};
	};
}

#endif // !KEY_EVENT_H_