#ifndef OCTREE_VOXEL_TERRAIN_RENDER_PROCEDURE_H_
#define OCTREE_VOXEL_TERRAIN_RENDER_PROCEDURE_H_

#include "TerrainDefines.h"

#include "RenderProcedure.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshIndexComponent.h"

#include "VoxelTerrain.h"
#include "Mutex.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API OctreeVoxelTerrainRenderProcedure : public RenderProcedure
	{
	public:

		OctreeVoxelTerrainRenderProcedure(VoxelTerrain* terrain);

		virtual ~OctreeVoxelTerrainRenderProcedure();

		virtual void render() override;

		virtual bool isValidProcedure()  override;

		void updateTerrain();

		void renderTerrain();

		void drawOctreeNode(TerrainOctree* octree, Shader* shader);

		void drawOctreeNodeTest(TerrainOctree* octree, MeshIndexComponent* component, Shader* shader);

	private:

		VoxelTerrain* _terrain;
		Mesh* _testMesh;
		Mesh* _terrainMesh;
		Material* _terrainMaterial;
		Mutex _terrainMutex;
	};

}

#endif // !OCTREE_VOXEL_TERRAIN_RENDER_PROCEDURE_H_

