#include "Message.h"

namespace Arcana
{

	Message::Message() : _event(nullptr)
	{
	}

	Message::Message(Event* event) : _event(event)
	{
		_event.AddReference();
		//LOG(Info, CoreEngine, "Creating initial message........ " + std::to_string(_event.getReferenceCount()));
	}

	Message::Message(const Message& message) : _event(message._event)
	{
		//LOG(Info, CoreEngine, "Message copy constructor........ " + std::to_string(_event.getReferenceCount()));
	}

	Message::~Message()
	{
		//LOG(Error, CoreEngine, "Message destructor (destroys event).");
		if (!_event.isNull())
		{
			//LOG(Error, CoreEngine, "EVENT NOT NULL...... " + std::to_string(_event.getReferenceCount()));
		}
	}

	void Message::setEvent(SmartPtr<Event> event)
	{
		_event = event;
	}

	Event& Message::getEvent()
	{
		if (_event.get() != nullptr)
		{
			//LOG(Error, CoreEngine, "EVENT NOT NULL IN GET EVENT...... " + std::to_string(_event.getReferenceCount()));
		}

		return *_event;
	}

	Message& Message::operator=(const Message& msg)
	{
		_event = msg._event;
		//LOG(Info, CoreEngine, "Message assignment operator...... " + std::to_string(_event.getReferenceCount()));
		return *this;
	}
}
