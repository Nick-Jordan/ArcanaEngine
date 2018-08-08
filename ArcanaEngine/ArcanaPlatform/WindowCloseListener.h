#ifndef WINDOW_CLOSE_LISTENER_H_
#define WINDOW_CLOSE_LISTENER_H_

#include "PlatformDefines.h"

#include "EventListener.h"
#include "CloseOperation.h"

namespace Arcana
{

	class Application;

	class ARCANA_PLATFORM_API WindowCloseListener : public EventListener
	{
	public:

		WindowCloseListener();

		~WindowCloseListener();


		virtual bool processEvent(Event& event, EventHandler& handler) override;


		void setCloseOperation(CloseOperation operation);

		CloseOperation getCloseOperation() const;

		void setParentApplication(Application* app);

	private:


		CloseOperation _closeOperation;

		Application* _parentApplication;

	};

}

#endif // !WINDOW_CLOSE_LISTENER_H_

