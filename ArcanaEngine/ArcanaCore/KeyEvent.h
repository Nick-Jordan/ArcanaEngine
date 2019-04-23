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
	
		/// Enum defining the event type.
		enum Type
		{
			Pressed, ///< Key was pressed.
			Released, ///< Key was released.
			Axis ///< Key is an axis.
		};
	
		/** \brief KeyEvent constructor.
		 */
		KeyEvent(Type event, int32 keyCode, bool alt, bool control, bool shift, bool system) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", event);
			getData().addInt("keyCode", keyCode);
			getData().addBool("alt", alt);
			getData().addBool("control", control);
			getData().addBool("shift", shift);
			getData().addBool("system", system);
		};

		/** \brief KeyEvent constructor.
		 */
		KeyEvent(Type event, int32 keyCode, int x, int y, bool alt, bool control, bool shift, bool system) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", event);
			getData().addInt("keyCode", keyCode);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addBool("alt", alt);
			getData().addBool("control", control);
			getData().addBool("shift", shift);
			getData().addBool("system", system);
		};

		/** \brief KeyEvent constructor for controller key presses
		 */
		KeyEvent(Type event, int32 controllerId, int32 keyCode) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", event);
			getData().addInt("controller", controllerId);
			getData().addInt("keyCode", keyCode);
		}

		/** \brief KeyEvent constructor for controller axis keys.
		 */
		KeyEvent(int32 controllerId, int32 keyCode, float axis) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", Axis);
			getData().addInt("controller", controllerId);
			getData().addInt("keyCode", keyCode);
			getData().addFloat("axis", axis);
		}

		/** \brief KeyEvent constructor for axis keys.
		 */
		KeyEvent(int32 keyCode, float axis) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", Axis);
			getData().addInt("keyCode", keyCode);
			getData().addFloat("axis", axis);
		}

		/*KeyEvent(int32 controllerId, int32 keyCode, double xAxis, double yAxis) : Event(EventID::KeyEventID)
		{
			getData().addInt("event", Axis);
			getData().addInt("controller", controllerId);
			getData().addInt("keyCode", keyCode);
			getData().addDouble("xAxis", xAxis);
			getData().addDouble("yAxis", yAxis);
		}*/
	};
}

#endif // !KEY_EVENT_H_