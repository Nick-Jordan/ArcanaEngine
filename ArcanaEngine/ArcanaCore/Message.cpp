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
			LOGF(Info, CoreEngine, "%d EVENT CREATED: %d, %d, %f", _event->getEventId(), _event->referenceCount(), _event->getInt("keyCode"), _event->getFloat("axis"));
			_event->reference();
		}
	}

	Message::Message(const Message& message) : _event(message._event)
	{
		LOG(Info, CoreEngine, "Message copy constructor");
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
			LOGF(Info, CoreEngine, "%d EVENT BEING RELEASED: %d, %d, %f", _event->getEventId(), _event->referenceCount(), _event->getInt("keyCode"), _event->getFloat("axis"));

			_event->release();
		}
	}

	void Message::setEvent(Event* event)
	{
		if (_event)
		{
			_event->release();
		}

		LOG(Error, CoreEngine, "set event");
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
