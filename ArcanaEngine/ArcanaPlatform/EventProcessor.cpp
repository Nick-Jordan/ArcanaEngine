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
		LOGF(Debug, WindowLog, "Messages left: %d", _messages.size());
	}

	bool EventProcessor::popMessage(WindowContext& window, Message& msg, bool block)
	{
		LOGF(Debug, WindowLog, "Message queue size: %d", _messages.size());

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

		if(!_messages.empty())
		{
			msg = _messages.front();
			_messages.pop();

			return true;
		}

		return false;
	}

	void EventProcessor::pushMessage(const Message& msg)
	{
		_messages.push(msg);
	}
}
