#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "CoreDefines.h"

#include "Event.h"
#include "SmartPtr.h"

namespace Arcana
{
	/** \brief This class manages the lifetime of an Event pointer.
	 */
	class ARCANA_CORE_API Message
	{
	public:

		/** \brief Message default constructor.
		 */
		Message();

		/** \brief Message event constructor.
		 *  
		 *  References and stores the event pointer.
		 */
		Message(Event* event);

		/** \brief Message copy constructor.
		 *
		 *  References and stores the event pointer.
		 */
		Message(const Message& message);
		//Message(Message&& message);

		/** \brief Message destructor.
		 *
		 *  Releases the event pointer.
		 */
		~Message();

		/** \brief Sets the event pointer.
		 */
		void setEvent(Event* event);

		/** \brief Dereferences the event pointer.
		 */
		Event& getEvent();

		/** \brief Message assignment operator.
		 */
		Message& operator=(const Message& msg);
		//Message& operator=(Message&& msg);

	private:

		Event* _event; ///< The event.
	};

}

#endif // !MESSAGE_H_

