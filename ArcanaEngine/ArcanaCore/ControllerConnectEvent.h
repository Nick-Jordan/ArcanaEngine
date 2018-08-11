#ifndef CONTROLLER_CONNECT_EVENT_H_
#define CONTROLLER_CONNECT_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"
#include "Types.h"

namespace Arcana
{
	struct ControllerConnectEvent : public Event
	{
		enum Type
		{
			Connected,
			Disconnected
		};

		ControllerConnectEvent(Type event, int32 controllerId) : Event(EventID::ControllerConnectEventID)
		{
			getData().addInt("event", event);
			getData().addInt("controller", controllerId);
		}
	};
}

#endif // !CONTROLLER_CONNECT_EVENT_H_
