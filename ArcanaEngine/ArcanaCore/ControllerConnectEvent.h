#ifndef CONTROLLER_CONNECT_EVENT_H_
#define CONTROLLER_CONNECT_EVENT_H_

#include "CoreDefines.h"

#include "Event.h"
#include "Types.h"

namespace Arcana
{
	/**\brief Controller connection event.
	 *
	 *  Broadcasted when a new controller is connected/disconnected.
	 *  Data includes the type (connected or disconnected) and the controller ID.
	 */

	struct ControllerConnectEvent : public Event
	{
		/// Controller event types.
		enum Type
		{
			Connected, ///< Controller connected.
			Disconnected ///< Controller disconnected.
		};

		/** \brief ControllerConnectEvent constructor.
		 */
		ControllerConnectEvent(Type event, int32 controllerId) : Event(EventID::ControllerConnectEventID)
		{
			getData().addInt("event", event);
			getData().addInt("controller", controllerId);
		}
	};
}

#endif // !CONTROLLER_CONNECT_EVENT_H_
