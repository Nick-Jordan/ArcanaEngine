#include "EventProcessor.h"
#include "WindowContext.h"

#include "Window.h"

namespace Arcana
{

	EventProcessor::EventProcessor() : _messages()
	{
	}


	EventProcessor::~EventProcessor()
	{
	}

	bool EventProcessor::popMessage(WindowContext& window, Message& msg, bool block)
	{
		//LOG(Debug, WindowLog, "Message array size: " + std::to_string(_messages.size()));

		if (_messages.empty())
		{
			window.processControllerEvents();
			window.processEvents();

			if (block)
			{

				while (_messages.empty())
				{
					//sleep
					window.processControllerEvents();
					window.processEvents();
				}
			}
		}

		if (!_messages.empty())
		{
			msg = _messages.front();
			//LOG(Debug, WindowLog, "Event type Pop Message (First Test) " + std::to_string(_messages.front().getEvent().getEventId()));
			//LOG(Debug, WindowLog, "Event type Pop Message (Second Test) " + std::to_string(msg.getEvent().getEventId()));
			_messages.pop();

			return true;
		}

		return false;
	}

	void EventProcessor::pushMessage(Message& msg)
	{
		//LOG(Debug, WindowLog, "Message pushed....");
		//LOG(Debug, WindowLog, "Message array size (During push): " + std::to_string(_messages.size()));
		_messages.push(msg);
		//LOG(Debug, WindowLog, "Message array size (During push): " + std::to_string(_messages.size()));

		//LOG(Debug, WindowLog, "Event type Push Message " + std::to_string(msg.getEvent().getEventId()));	
	}
}
