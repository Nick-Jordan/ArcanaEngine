#ifndef EVENT_PROCESSOR_H_
#define EVENT_PROCESSOR_H_

#include "PlatformDefines.h"

//#include "Array.h"
#include "Message.h"

#include <queue>

namespace Arcana
{

	class WindowContext;

	class ARCANA_PLATFORM_API EventProcessor
	{
	public:

		EventProcessor();

		~EventProcessor();

		bool popMessage(WindowContext& window, Message& msg, bool block);

		void pushMessage(const Message& msg);
		
	private:

		std::queue<Message> _messages;
	};

}

#endif // !EVENT_PROCESSOR_H_

