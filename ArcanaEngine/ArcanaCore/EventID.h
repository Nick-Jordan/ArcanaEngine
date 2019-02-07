#ifndef EVENT_ID_H_
#define EVENT_ID_H_

#include "Types.h"

namespace Arcana
{
	/// Enum denoting the event type.
	enum EventID : uint64
	{
		DefaultID = 0,  ///< Default events
		KeyEventID,  ///< Key events
		MouseEventID,  ///< Mouse events
		WindowClosedEventID,  ///< Window closed events
		ControllerConnectEventID,  ///< Controller connected/disconnected events

		NUM_ENGINE_EVENTS  ///< The total number of ArcanaEngine events
	};
}

#endif // !EVENT_ID_H_
