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
	
		struct Data : public Event::Data
		{
			Type event;
			uint64 keyCode;//Key
			bool alt;
			bool control;
			bool shift;
			bool system;

			Data() {};
			Data(Type cevent, uint64 ckeyCode, bool calt, bool ccontrol, bool cshift, bool csystem)
				: event(cevent), keyCode(ckeyCode), alt(calt), control(ccontrol), shift(cshift), system(csystem)
			{};
		};
	
		KeyEvent(Type event, uint64 keyCode, bool alt, bool control, bool shift, bool system) : Event(1)
		{
			setData(new Data(event, keyCode, alt, control, shift, system));
		};
	};
}

#endif // !KEY_EVENT_H_