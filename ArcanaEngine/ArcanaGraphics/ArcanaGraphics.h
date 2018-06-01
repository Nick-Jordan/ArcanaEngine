#ifndef ARCANA_GRAPHICS_H_
#define ARCANA_GRAPHICS_H_

#include "GraphicsDefines.h"

#include "ModuleInterface.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API ArcanaGraphics : public ModuleInterface
	{
	public:

		ArcanaGraphics();

		virtual ~ArcanaGraphics();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif