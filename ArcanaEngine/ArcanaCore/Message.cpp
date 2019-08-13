#include "Message.h"

namespace Arcana
{

	Message::Message() : _event(nullptr)
	{
	}

	Message::Message(Event* event) : _event(event)
	{
		AE_REFERENCE(_event);
	}

	Message::Message(const Message& message) : _event(message._event)
	{
		AE_REFERENCE(_event);
	}

	/*Message::Message(Message&& message) : _event(message._event)
	{
		//message._event = nullptr;
	}*/

	Message::~Message()
	{
		if (_event)
		{
			_event->release();
		}
	}

	void Message::setEvent(Event* event)
	{
		AE_RELEASE(_event);
		_event = event;
		AE_REFERENCE(_event);
	}

	Event& Message::getEvent()
	{
		if (_event != nullptr)
		{
			return *_event;
		}

		return Event();
	}

	Message& Message::operator=(const Message& msg)
	{
		AE_RELEASE(_event);

		_event = msg._event;

		//*this = Message(msg._event);

		AE_REFERENCE(_event);

		return *this;
	}

	/*Message& Message::operator=(Message&& msg)
	{
		_event = msg._event;

		//msg._event = nullptr;

		return *this;
	}*/
}
