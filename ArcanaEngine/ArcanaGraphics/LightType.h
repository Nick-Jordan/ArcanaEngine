#ifndef LIGHT_TYPE_H_
#define LIGHT_TYPE_H_

#include "GraphicsDefines.h"

namespace Arcana
{
	enum ARCANA_GRAPHICS_API LightType
	{
		Directional,
		Point,
		Spot,
		Rectangle,
		NumLightTypes
	};
}

#endif // !LIGHT_TYPE_H_