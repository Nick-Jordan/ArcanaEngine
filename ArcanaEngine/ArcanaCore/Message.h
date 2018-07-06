#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "CoreDefines.h"

#include "Event.h"
#include "SmartPtr.h"

namespace Arcana
{

	class ARCANA_CORE_API Message
	{
	public:

		Message();
		Message(Event* event);
		Message(const Message& message);
		~Message();

		void setEvent(SmartPtr<Event> event);
		Event& getEvent();

		Message& operator=(const Message& msg);

	private:

		SmartPtr<Event> _event;
	};

}

#endif // !MESSAGE_H_

