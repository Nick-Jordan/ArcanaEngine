#ifndef RENDER_LIGHT_H_
#define RENDER_LIGHT_H_

#include "GraphicsDefines.h"

#include "Vector3.h"
#include "LightProperties.h"

namespace Arcana
{
	struct ARCANA_GRAPHICS_API RenderLight
	{
		Vector3f position;
		Vector3f color;
		float intensity;

		int32 type;
		LightProperties lightProperties;
	};
}

#endif // !RENDER_LIGHT_H_