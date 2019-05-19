#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "TerrainNode.h"
#include "TerrainQuadRenderData.h"
#include "ObjectRenderer.h"

#include "TileSampler.h"
#include "Scheduler.h"
#include "Callback.h"

namespace Arcana
{
	REGISTER_CALLBACK_RETURN_TYPE(VertexTransformFunction, Vector3f, Vector3f);

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

		void getTerrainQuadVector(Mesh* mesh, Material* material);

	public:

		static void createAtmosphereTextures();

		static void createGrid(int32 size, std::vector<Vector3f>& vertices, std::vector<unsigned int>& indices, 
			Vector3f priorScale = Vector3f::one(), 
			Vector3f priorTranslation = Vector3f::zero(), 
			VertexTransformFunction function = VertexTransformFunction());

	private:

		void drawQuad(TerrainQuad* quad, Mesh* mesh, Material* material);

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

