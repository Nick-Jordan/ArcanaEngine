#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include "CoreDefines.h"

#include "Event.h"

#include "SmartPtr.h"
#include "EventListener.h"
#include "Object.h"
#include "Types.h"
#include "Array.h"

#include <string>

#include <memory>

namespace Arcana
{

	/** \brief Main class for broadcasting events.
	 *
	 *  
	 */
	
	class ARCANA_CORE_API EventHandler : public Object
	{
	public:
	
		/** \brief Typedef for an array of event listeners.
		 */

		//typedef Array<SmartPtr<EventListener>> ListenerArray;
		typedef Array<std::shared_ptr<EventListener>> ListenerArray;

		/** \brief Enum containing codes for event broadcast success and failure.
		 */

		enum BroadcastResultCode : uint8
		{
			EVENT_SUCCESS,
			EVENT_ERROR
		};

		/** \brief This class wraps a broadcast result code.
		 *  It contains methods for easily checking success and getting code strings for debug printing.
		 */

		class BroadcastResult
		{
		public:
			
			/** \brief BroadcastResult uint8 constructor.
			 */

			BroadcastResult(uint8 code) : _code(code) {};

			/** \brief BroadcastResult BroadcastResultCode constructor.
			 */

			BroadcastResult(BroadcastResultCode code) : _code(code) {};

			/** \brief Accessor for the result code.
			 */

			uint8 getCode() const { return _code; };

			/** \brief Returns the code string for printing.
			 */

			std::string getCodeString() const
			{
				std::string codeStrings[2] = { "Success", "Error" };
				return _code < 2 ? codeStrings[_code] : "Unknown";
			};

			/** \brief Returns true if the event broadcast succeeded.
			 */

			bool isSuccess() const { return _code == EVENT_SUCCESS; };
			
			/** \brief BroadcastResult boolean conversion operator.
			 */

			operator bool() const { return isSuccess(); };

		private:

			uint8 _code;  ///< The result code stored as an unsigned byte.
		};


		/** \brief EventHandler default constructor.
		 */

		EventHandler();

		/** \brief EventHandler destructor.
		 */
		
		~EventHandler();

		/** \brief Broadcasts the event to the event listeners and executes the event callback.
		 */

		BroadcastResult broadcast(Event& event);
		
		/** \brief Adds an event listener to the listener array.
		 */

		//void addEventListener(SmartPtr<EventListener> ptr);
		void addEventListener(std::shared_ptr<EventListener> ptr);
		
		/** \brief Removes and event listener from the listener array.
		 *  Returns the index of the listener in the array.
		 */

		//int32 removeEventListener(SmartPtr<EventListener> ptr);

		void test();
		
	private:
	
		ListenerArray _listeners;  ///< The array of EventListeners.
	};

}

#endif // !EVENT_HANDLER_H_

