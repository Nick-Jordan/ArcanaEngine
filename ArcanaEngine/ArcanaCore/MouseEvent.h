#ifndef MOUSE_EVENT_H_
#define MOUSE_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"

namespace Arcana
{
	class ARCANA_CORE_API MouseEvent : public Event
	{
	public:
	
		enum Type
		{
			WheelMoved,
			ButtonPressed,
			ButtonReleased,
			Moved,
			Entered,
			Exited
		};
	
		MouseEvent(Type event, int x, int y, uint64 button) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", event);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addInt("button", button);
		};
		
		MouseEvent(Type event, int x, int y, int delta) : Event(EventID::MouseEventID)
		{
			getData().addInt("event", event);
			getData().addInt("x", x);
			getData().addInt("y", y);
			getData().addInt("delta", delta);
		};
	};
}

#endif // !KEY_EVENT_H_