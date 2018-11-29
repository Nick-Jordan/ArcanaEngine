#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "PCGDefines.h"

#include "Object.h"
#include "TerrainNode.h"
#include "TerrainQuadRenderData.h"

namespace Arcana
{
	class ARCANA_PCG_API Terrain : public Object
	{
	public:

		Terrain();

		virtual ~Terrain();

		void getTerrainQuadVector(std::vector<TerrainQuadRenderData>& terrainQuadVector);

	private:

		void Terrain::drawQuad(TerrainQuad* quad, std::vector<TerrainQuadRenderData>& terrainQuadVector);

	public:

		TerrainNode* _terrainNode;

		bool _culling;
	};

}

#endif // !TERRAIN_H_

