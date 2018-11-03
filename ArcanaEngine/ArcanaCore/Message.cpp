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

	Message::~Message()
	{
		if (_event)
		{
			_event->release();
		}
	}

	void Message::setEvent(Event* event)
	{
		_event = event;
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
		_event = msg._event;

		if (_event)
		{
			_event->reference();
		}

		return *this;
	}
}
