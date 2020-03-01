#ifndef VOXEL_TERRAIN_RENDER_PROCEDURE_H_
#define VOXEL_TERRAIN_RENDER_PROCEDURE_H_

#include "TerrainDefines.h"

#include "RenderProcedure.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"

#include "VoxelTerrain.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API VoxelTerrainRenderProcedure : public RenderProcedure
	{
	public:

		VoxelTerrainRenderProcedure(const class Transform& transform);

		virtual ~VoxelTerrainRenderProcedure();

		virtual void render() override;

		virtual bool isValidProcedure()  override;

		void updateTerrain();

		void renderTerrain();

	private:

		Mesh* _mesh;
		Material* _terrainMaterial;
		VertexFormat _instanceFormat;

		Texture* _terrainSurface;
		Texture* _terrainColor;
	};

}

#endif // !VOXEL_TERRAIN_RENDER_PROCEDURE_H_

