#ifndef KEY_EVENT_H_
#define KEY_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"

namespace Arcana
{
	class KeyEvent : public Event
	{
	public:
	
		enum Type
		{
			Pressed,
			Released
		};
	
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

		static void eventCallback()
		{
			LOG(Info, CoreEngine, "KeyEvent broadcasted.");
		};
	};
}

#endif // !KEY_EVENT_H_