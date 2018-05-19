#ifndef KEY_EVENT_H_
#define KEY_EVENT_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Event.h"

namespace Arcana
{
	class ARCANA_CORE_API KeyEvent : public Event
	{
	public:
	
		enum Type
		{
			Pressed,
			Released
		};
	
		KeyEvent(Type event, uint64 keyCode, bool alt, bool control, bool shift, bool system) : Event(1)
		{
			getData().addInt("event", event);
			getData().addInt("keyCode", keyCode);
			getData().addBool("alt", alt);
			getData().addBool("control", control);
			getData().addBool("shift", shift);
			getData().addBool("system", system);
		};

		~KeyEvent()
		{
			LOG(Error, CoreEngine, "KeyEvent Destructed");
		};
	};
}

#endif // !KEY_EVENT_H_