#ifndef APPLICATION_CONTEXT_H_
#define APPLICATION_CONTEXT_H_

#include "PlatformDefines.h"

#include "ApplicationDefinition.h"

namespace Arcana
{

	class ARCANA_PLATFORM_API ApplicationContext
	{
	public:

		ApplicationContext() { };

		virtual ~ApplicationContext() { };

		virtual bool create(const ApplicationDefinition& definition) = 0;
	};


	class ARCANA_PLATFORM_API TestApplicationContext : public ApplicationContext
	{
		bool create(const ApplicationDefinition& definition)
		{
			return false;
		}
	};
}

#endif // !APPLICATION_CONTEXT_H_

