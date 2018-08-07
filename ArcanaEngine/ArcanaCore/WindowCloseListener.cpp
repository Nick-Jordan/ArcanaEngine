#include "WindowCloseListener.h"

namespace Arcana
{

	WindowCloseListener::WindowCloseListener()
	{
		listenForEvent(EventID::WindowClosedEventID);
	}


	WindowCloseListener::~WindowCloseListener()
	{
	}

	bool WindowCloseListener::processEvent(Event& event, EventHandler& handler)
	{

	}

}
