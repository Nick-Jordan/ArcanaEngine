#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "PlatformDefines.h"

#include "ApplicationContext.h"
#include "ArcanaLog.h"
#include "Window.h"

#include "EventHandler.h"
#include "WindowCloseListener.h"

#include <memory>

//vld
#include <vld.h>

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_PLATFORM_API, ApplicationLog, none)

	//REGISTER_CALLBACK(UpdateCallback)

	class ARCANA_PLATFORM_API Application
	{
	public:

		Application();

		Application(const ApplicationDefinition& definition);
		
		virtual ~Application();

		void initialize();

		bool create(const ApplicationDefinition& definition);

		const ApplicationDefinition& getApplicationDefinition() const;

		void setDefinition(ApplicationDefinition* definition, bool reinitialize = false);

		Window& getActiveWindow();

		EventHandler& getEventHandler();

		void setCloseOperation(CloseOperation operation);

		CloseOperation getCloseOperation() const;

		void start();

		//void checkSyncTasks();

	private:

		ApplicationContext* _applicationContext;

		const ApplicationDefinition* _definition;

		WindowCloseListener* _windowCloseListener;

		Array<Window> _windows;

		//std::queue<Message> _messageQueue;
		//std::mutex _messageQueueMutex;

		EventHandler _eventHandler;
	};
}

#endif // !APPLICATION_H_

