#include "WindowCloseListener.h"

#include "Application.h"
#include "ArcanaLog.h"

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
		LOG(Info, ApplicationLog, "WindowClosedListener processing event...");
		return _closeOperation.performOperation(_parentApplication->getActiveWindow());
	}

	void WindowCloseListener::setCloseOperation(CloseOperation operation)
	{
		_closeOperation = operation;
	}

	CloseOperation WindowCloseListener::getCloseOperation() const
	{
		return _closeOperation;
	}

	void WindowCloseListener::setParentApplication(Application* app)
	{
		_parentApplication = app;
	}

}
