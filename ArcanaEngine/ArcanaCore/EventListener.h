#ifndef EVENT_LISTENER_H_
#define EVENT_LISTENER_H_

#include "CoreDefines.h"

#include "Array.h"
#include "Types.h"

#include "Event.h"

namespace Arcana
{
	/** \brief EventHandler forward declaration.
	 */

	class ARCANA_CORE_API EventHandler;

	/** \brief An interface for processing events.
	 *  
	 *  This is the base class for all other classes that need to process events.
	 *  There are methods for setting specific event ids to listen for.
	 *  This eliminates the need to check the event id in the process event method.
	 */

	class ARCANA_CORE_API EventListener
	{
	public:
	
		/** \brief EventListener virtual destructor.
		 */

		virtual ~EventListener() {};

		/** \brief Pure virtual process event function.
		 *  This function must be overriden in all derived classes.
		 */
		
		virtual bool processEvent(Event& event, EventHandler& handler) = 0;
		
		/** \brief Adds an event id to the list of listened events.
		 *  The event listener will listen for events with the specified id.
		 */
		
		void listenForEvent(uint64 eventId)
		{
			_eventIds.add(eventId);
		}
		
		/** \brief Returns true if the listener is listening for events with this id.
		 */

		bool isListeningForEvent(uint64 eventId) const
		{
			return _eventIds.contains(eventId);
		}
		
	private:
	
		Array<uint64> _eventIds;  ///< The array of event ids that the listener looks for.
	};
}

#endif // !EVENT_LISTENER_H_