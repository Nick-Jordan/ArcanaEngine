#ifndef MODULE_INTERFACE_H_
#define MODULE_INTERFACE_H_

#include "ArcanaCoreAPIDefinition.h"

namespace Arcana
{
	class ARCANA_CORE_API ModuleInterface
	{
	public:

		virtual ~ModuleInterface() { };

		virtual bool startUp() = 0;

		virtual bool shutDown() = 0;

		virtual bool isGameModule() = 0;
	};
}

#endif
