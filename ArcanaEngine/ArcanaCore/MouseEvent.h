#ifndef MOUSE_EVENT_H_
#define MOUSE_EVENT_H_

#include "ArcanaCoreAPIDefinition.h"

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
	
		struct Data : public Event::Data
		{
			Type event;
			int x;
			int y;
			union
			{
				uint64 button;//Button
				int delta;
			};
		};
	
		MouseEvent(Type event, int x, int y, uint64 button) : Event(2)
		{
			//setData(new Data(event, x, y, button));
		};
		
		MouseEvent(Type event, int x, int y, int delta) : Event(2)
		{
			//setData(new Data(event, x, y, delta));
		};
	};
}

#endif // !KEY_EVENT_H_