#ifndef KEY_LISTENER_H_
#define KEY_LISTENER_H_

#include "ArcanaCoreAPIDefinition.h"

#include "EventListener.h"
#include "EventHandler.h"

namespace Arcana
{
	
	class ARCANA_CORE_API KeyListener : public EventListener
	{
	public:
	
		KeyListener();
		
		virtual ~KeyListener();
		
		virtual bool processEvent(Event& event, EventHandler& handler);
	};

}

#endif // !KEY_LISTENER_H_