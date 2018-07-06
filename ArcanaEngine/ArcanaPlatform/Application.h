#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "PlatformDefines.h"

#include "ApplicationContext.h"
#include "ArcanaLog.h"
#include "Window.h"

#include "EventHandler.h"

#include <memory>

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_PLATFORM_API, ApplicationLog, none)

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


		void start();

	private:

		ApplicationContext* _applicationContext;

		const ApplicationDefinition* _definition;

		Array<Window> _windows;

		EventHandler _eventHandler;
	};
}

#endif // !APPLICATION_H_

