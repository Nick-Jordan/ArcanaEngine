#include "Message.h"

namespace Arcana
{

	Message::Message() : _event(nullptr)
	{
	}

	Message::Message(Event* event) : _event(event)
	{
		if (_event)
		{
			_event->reference();
		}
	}

	Message::Message(const Message& message) : _event(message._event)
	{
		if (_event)
		{
			_event->reference();
		}
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
		if (_event)
		{
			_event->release();
		}

		_event = event;
		_event->reference();
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
		if (_event)
		{
			_event->release();
		}

		_event = msg._event;

		//*this = Message(msg._event);

		if (_event)
		{
			_event->reference();
		}

		return *this;
	}

	/*Message& Message::operator=(Message&& msg)
	{
		_event = msg._event;

		//msg._event = nullptr;

		return *this;
	}*/
}
