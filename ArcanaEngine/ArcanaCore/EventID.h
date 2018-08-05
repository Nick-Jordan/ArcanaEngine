#ifndef EVENT_ID_H_
#define EVENT_ID_H_

#include "Types.h"

namespace Arcana
{
	enum EventID : uint64
	{
		DefaultID = 0,
		KeyEventID,
		MouseEventID,
		WindowClosedEventID,

		NUM_ENGINE_EVENTS
	};
}

#endif // !EVENT_ID_H_
