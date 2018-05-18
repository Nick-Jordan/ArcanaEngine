#ifndef MOUSE_LISTENER_H_
#define MOUSE_LISTENER_H_

#include "ArcanaCoreAPIDefinition.h"

#include "EventListener.h"
#include "EventHandler.h"


namespace Arcana
{
	
	class ARCANA_CORE_API MouseListener : public EventListener
	{
	public:
	
		MouseListener();
		
		virtual ~MouseListener();
		
		virtual bool processEvent(Event& event, EventHandler& handler);
	};

}

#endif // !KEY_LISTENER_H_