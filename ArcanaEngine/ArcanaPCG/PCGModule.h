#ifndef PCG_MODULE_H_
#define PCG_MODULE_H_

#include "PCGDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{
	class ARCANA_PCG_API PCGModule : public ModuleInterface
	{
	public:

		PCGModule();

		virtual ~PCGModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif