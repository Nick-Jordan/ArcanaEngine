#ifndef TERRAIN_RENDER_PROCEDURE_H_
#define TERRAIN_RENDER_PROCEDURE_H_

#include "PCGDefines.h"

#include "RenderProcedure.h"
#include "Terrain.h"

namespace Arcana
{
	class ARCANA_PCG_API TerrainRenderDataUpdate : public RenderDataUpdate
	{
	public:

	};

	class ARCANA_PCG_API TerrainRenderData : public RenderData
	{
	public:

		TerrainRenderData(Terrain* terrain);

		~TerrainRenderData();

		virtual void render(ObjectRenderer& renderer) override;

	public:

		Terrain* _terrain;
		MeshRenderContext _context;
		std::mutex _meshQueueMutex;
		Texture* _testTexture0;
		Texture* _testTexture1;
	};

	class ARCANA_PCG_API TerrainRenderProcedure : public RenderProcedure
	{
	public:

		TerrainRenderProcedure(Terrain* terrain);

		virtual ~TerrainRenderProcedure();

		virtual bool isDirty() const override;

		virtual void markDirty(bool dirty) override;

		virtual void createRenderData()  override;

		virtual void updateRenderData(const RenderDataUpdate& data)  override;

		virtual RenderData* getRenderData() const  override;

		virtual bool isValidProcedure()  override;

		void updateTerrain();

	private:

		static void createGrid(std::vector<Vector3f>& vertices, std::vector<unsigned int>& indices);

	private:

		TerrainRenderData* _data;
		Terrain* _tempTerrain;
		Mesh* _mesh;
		Material* _terrainMaterial;
		RenderState _terrainRenderState;
	};

}

#endif // !TERRAIN_RENDER_PROCEDURE_H_

