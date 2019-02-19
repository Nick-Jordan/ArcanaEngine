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
