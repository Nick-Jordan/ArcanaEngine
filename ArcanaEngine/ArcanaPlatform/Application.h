#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "PlatformDefines.h"

#include "ApplicationContext.h"
#include "ArcanaLog.h"
#include "Window.h"

#include <memory>

namespace Arcana
{
	REGISTER_CATEGORY(ApplicationLog, none)

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

	private:

		ApplicationContext* _applicationContext;

		const ApplicationDefinition* _definition;

		Array<Window> _windows;
	};
}

#endif // !APPLICATION_H_

