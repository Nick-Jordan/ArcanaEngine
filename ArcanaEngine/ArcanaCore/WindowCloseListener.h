#ifndef WINDOW_CLOSE_LISTENER_H_
#define WINDOW_CLOSE_LISTENER_H_

#include "CoreDefines.h"

#include "EventListener.h"

namespace Arcana
{

	class ARCANA_CORE_API WindowCloseListener : public EventListener
	{
	public:

		WindowCloseListener();

		~WindowCloseListener();


		virtual bool processEvent(Event& event, EventHandler& handler) override;
	};

}

#endif // !WINDOW_CLOSE_LISTENER_H_

