#ifndef PHYSICS_MODULE_H_
#define PHYSICS_MODULE_H_

#include "PhysicsDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{
	class ARCANA_PHYSICS_API PhysicsModule : public ModuleInterface
	{
	public:

		PhysicsModule();

		virtual ~PhysicsModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;

		virtual const char* getModuleName() const override;
	};
}

#endif