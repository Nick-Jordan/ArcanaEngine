#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "TerrainNode.h"
#include "TerrainQuadRenderData.h"
#include "ObjectRenderer.h"

#include "TileSampler.h"
#include "Scheduler.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API Terrain : public Object
	{
	public:

		struct ARCANA_TERRAIN_API Parameters
		{
			std::string deformation;
			double radius;
			double splitFactor;
			float zmin;
			float zmax;
			int32 maxLevel;
		};

		Terrain(const Parameters& params);

		virtual ~Terrain();

		void getTerrainQuadVector(const MeshRenderContext& data);

	private:

		void drawQuad(TerrainQuad* quad, const MeshRenderContext& data);

	public:

		static Texture* _inscatter;
		static Texture* _irradiance;
		static Texture* _transmittance;
		static Texture* _sunglare;

	public:

		Scheduler* scheduler;

		TerrainNode* _terrainNode;

		bool _culling;

		Array<TileSampler*> _tileSamplers;
	};

}

#endif // !TERRAIN_H_

