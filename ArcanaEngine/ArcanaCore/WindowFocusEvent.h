#ifndef WINDOW_FOCUS_EVENT_H_
#define WINDOW_FOCUS_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"

namespace Arcana
{
	class WindowFocusEvent : public Event
	{
	public:

		enum Type
		{
			Lost,
			Gained,
		};

		WindowFocusEvent(Type type) : Event(EventID::WindowFocusEventID)
		{
			getData().addInt("type", type);
		};
	};
}

#endif // !KEY_EVENT_H_