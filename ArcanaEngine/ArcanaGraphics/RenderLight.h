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
		Vector3f direction;
		Vector3f color;
		float sourceWidth;
		float sourceHeight;
		float sourceRadius;
		float softSourceRadius;
		float intensity;
		float innerAngle;
		float outerAngle;
		float constant;
		float linear;
		float quadratic;

		int32 type;
		int32 mobility;
		LightProperties lightProperties;
		uint64 id;
	};
}

#endif // !RENDER_LIGHT_H_