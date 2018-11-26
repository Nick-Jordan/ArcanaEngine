#ifndef MOUSE_EVENT_H_
#define MOUSE_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"

namespace Arcana
{
	class MouseEvent : public Event
	{
	public:
	
		enum Type
		{
			WheelMoved,
			WheelScrolled,
			Moved,
			Entered,
			Exited
		};

		enum Wheel
		{
			Vertical,
			Horizontal
		};
	
		MouseEvent(Type event, int x, int y, uint64 button) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", event);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addInt("button", button);
		};
		
		MouseEvent(int x, int y, int delta) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", WheelMoved);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addInt("delta", delta);
		};

		MouseEvent(int x, int y, float delta, Wheel wheel) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", WheelScrolled);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addFloat("delta", delta);
			getData().addInt("wheel", wheel);
		};

		MouseEvent(int x, int y) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", Moved);
			getData().addInt("x", x);
			getData().addInt("y", y);
		}

		MouseEvent(Type event) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", event);
		}
	};
}

#endif // !KEY_EVENT_H_