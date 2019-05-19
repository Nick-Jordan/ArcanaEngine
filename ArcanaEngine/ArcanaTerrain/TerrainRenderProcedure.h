#ifndef TERRAIN_RENDER_PROCEDURE_H_
#define TERRAIN_RENDER_PROCEDURE_H_

#include "TerrainDefines.h"

#include "RenderProcedure.h"
#include "Terrain.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TerrainRenderDataUpdate : public RenderDataUpdate
	{
	public:

	};

	class ARCANA_TERRAIN_API TerrainRenderData : public RenderData
	{
	public:

		TerrainRenderData(Terrain* terrain);

		~TerrainRenderData();

		virtual void render(ObjectRenderer& renderer) override;

	public:

		Terrain* _terrain;
		Material* _terrainMaterial;
		MeshRenderContext _context;
		std::mutex _meshQueueMutex;

		Texture* _terrainSurface;
		Texture* _terrainColor;

		float t = 0.0f;
	};

	class ARCANA_TERRAIN_API TerrainRenderProcedure : public RenderProcedure
	{
	public:

		TerrainRenderProcedure(Terrain* terrain, const Transform& transform, std::string vertex, std::string fragment);

		virtual ~TerrainRenderProcedure();

		virtual bool isDirty() const override;

		virtual void markDirty(bool dirty) override;

		virtual void createRenderData()  override;

		virtual void updateRenderData(const RenderDataUpdate& data)  override;

		virtual RenderData* getRenderData() const  override;

		virtual bool isValidProcedure()  override;

		void updateTerrain();

		void renderTerrain();

	private:

		TerrainRenderData* _data;
		Terrain* _tempTerrain;
		Mesh* _mesh;
		Material* _terrainMaterial;
		RenderState _terrainRenderState;
		Transform _transform;
		std::string _vertex;
		std::string _fragment;
		VertexFormat _instanceFormat;
	};

}

#endif // !TERRAIN_RENDER_PROCEDURE_H_

