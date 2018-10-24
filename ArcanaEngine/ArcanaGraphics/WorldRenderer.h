#ifndef WORLD_RENDERER_H_
#define WORLD_RENDERER_H_

#include "GraphicsDefines.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API WorldRenderer
	{
	public:

		virtual void renderActors() = 0;
	};
}

#endif // !WORLD_RENDERER_H_