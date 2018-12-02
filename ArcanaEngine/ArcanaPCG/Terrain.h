#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "PCGDefines.h"

#include "Object.h"
#include "TerrainNode.h"
#include "TerrainQuadRenderData.h"
#include "ObjectRenderer.h"

namespace Arcana
{
	class ARCANA_PCG_API Terrain : public Object
	{
	public:

		Terrain();

		virtual ~Terrain();

		void getTerrainQuadVector(const MeshRenderContext& data);

	private:

		void drawQuad(TerrainQuad* quad, const MeshRenderContext& data);

	public:

		TerrainNode* _terrainNode;

		bool _culling;
	};

}

#endif // !TERRAIN_H_

