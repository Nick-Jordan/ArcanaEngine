#ifndef FTL_RESULT_H_
#define FTL_RESULT_H_

#include "GraphicsDefines.h"

#include "FTLGlobalIlluminationTexture.h"

namespace Arcana
{
	struct ARCANA_GRAPHICS_API FTLResult
	{
		Texture* LightMap;
		FTLGlobalIlluminationTexture IndirectLightData;

		FTLResult() : LightMap(nullptr) {}
	};
}

#endif // !FTL_RESULT_H_