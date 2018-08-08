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
			Released,
			Axis
		};
	
		/** \brief KeyEvent constructor.
		 */

		KeyEvent(Type event, int32 keyCode, bool alt = false, bool control = false, bool shift = false, bool system = false) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", event);
			getData().addInt("keyCode", keyCode);
			getData().addBool("alt", alt);
			getData().addBool("control", control);
			getData().addBool("shift", shift);
			getData().addBool("system", system);

			getEventCallback().bind(eventCallback);
		};

		KeyEvent(int32 controllerId, int32 keyCode, double axis) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", Axis);
			getData().addInt("controller", controllerId);
			getData().addInt("keyCode", keyCode);
			getData().addDouble("axis", axis);
		}

		KeyEvent(int32 controllerId, int32 keyCode, double xAxis, double yAxis) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", Axis);
			getData().addInt("controller", controllerId);
			getData().addInt("keyCode", keyCode);
			getData().addDouble("xAxis", xAxis);
			getData().addDouble("yAxis", yAxis);
		}

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