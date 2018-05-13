#ifndef MODULE_MANAGER_H_
#define MODULE_MANAGER_H_

#include "ArcanaCoreAPIDefinition.h"

namespace Arcana
{
	class ARCANA_CORE_API ModuleManager
	{
	public:

		ModuleManager();

		~ModuleManager();

		virtual bool startUp() = 0;

		virtual bool shutDown() = 0;
	};
}

#endif
