#ifndef TERRAIN_QUAD_RENDER_DATA_H_
#define TERRAIN_QUAD_RENDER_DATA_H_

#include "TerrainDefines.h"

#include "MeshRenderContext.h"
#include <vector>

namespace Arcana
{
	class ARCANA_TERRAIN_API TerrainQuadRenderData
	{
	public:

		std::vector<MeshRenderContext::UniformParameter> uniforms;
	};
}

#endif // !TERRAIN_QUAD_RENDER_DATA_H_