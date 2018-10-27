#ifndef WORLD_RENDERER_H_
#define WORLD_RENDERER_H_

#include "GraphicsDefines.h"

#include "Object.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API WorldRenderer : public Object
	{
	public:

		virtual void renderActors() = 0;
	};
}

#endif // !WORLD_RENDERER_H_