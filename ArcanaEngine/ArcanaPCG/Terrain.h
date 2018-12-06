#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "PCGDefines.h"

#include "Object.h"
#include "TerrainNode.h"
#include "TerrainQuadRenderData.h"
#include "ObjectRenderer.h"

#include "TileSampler.h"
#include "Scheduler.h"

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

		Scheduler* scheduler;

		TerrainNode* _terrainNode;

		bool _culling;

		Array<TileSampler*> _tileSamplers;
	};

}

#endif // !TERRAIN_H_

