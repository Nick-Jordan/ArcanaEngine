#ifndef MOUSE_EVENT_H_
#define MOUSE_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"

namespace Arcana
{

	/** \brief Event broadcasted on mouse presses/releases/movement/etc.
	 *
	 *  MouseEvents have and event id of 2.
	 */
	class MouseEvent : public Event
	{
	public:
	
		/// Mouse event type.
		enum Type
		{
			WheelMoved, ///< Mouse wheel moved.
			WheelScrolled, ///< Mouse wheel scrolled.
			Moved, ///< Mouse moved.
			Entered, ///< Mouse ented the window.
			Exited ///< Mouse ented the exited.
		};

		/// Mouse wheel ID.
		enum Wheel
		{
			Vertical, ///< Vertical wheel.
			Horizontal ///< Horizontal wheel.
		};
		
		/** \brief MouseEvent button press constructor.
		 */
		MouseEvent(Type event, int x, int y, uint64 button) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", event);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addInt("button", button);
		};
		
		/** \brief MouseEvent wheel moved constructor.
		 */
		MouseEvent(int x, int y, int delta) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", WheelMoved);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addInt("delta", delta);
		};

		/** \brief MouseEvent wheel scrolled constructor.
		 */
		MouseEvent(int x, int y, float delta, Wheel wheel) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", WheelScrolled);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addFloat("delta", delta);
			getData().addInt("wheel", wheel);
		};

		/** \brief MouseEvent moved constructor.
		 */
		MouseEvent(int x, int y) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", Moved);
			getData().addInt("x", x);
			getData().addInt("y", y);
		}

		/** \brief MouseEvent constructor.
		 */
		MouseEvent(Type event) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", event);
		}
	};
}

#endif // !KEY_EVENT_H_