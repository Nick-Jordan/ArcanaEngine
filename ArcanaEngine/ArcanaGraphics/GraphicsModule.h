#ifndef GRAPHICS_MODULE_H_
#define GRAPHICS_MODULE_H_

#include "GraphicsDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{
	class ARCANA_GRAPHICS_API GraphicsModule : public ModuleInterface
	{
	public:

		GraphicsModule();

		virtual ~GraphicsModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;

		virtual const char* getModuleName() const override;
	};
}

#endif