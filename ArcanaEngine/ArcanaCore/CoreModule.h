#ifndef CORE_MODULE_H_
#define CORE_MODULE_H_

#include "ArcanaCoreAPIDefinition.h"

#include "ModuleInterface.h"

namespace Arcana
{
	class CoreModule : public ModuleInterface
	{
	public:

		CoreModule();

		~CoreModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif