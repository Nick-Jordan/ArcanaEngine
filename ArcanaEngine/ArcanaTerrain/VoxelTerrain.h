#ifndef VOXEL_TERRAIN_H_
#define VOXEL_TERRAIN_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "ObjectRenderer.h"
#include "TerrainOctree.h"
#include "Scheduler.h"

#include <map>

namespace Arcana
{
	class ARCANA_TERRAIN_API VoxelTerrain : public Object
	{
		friend class OctreeVoxelTerrainRenderProcedure;

	public:

		VoxelTerrain();

		virtual ~VoxelTerrain();

		void update(Matrix4d transform, Matrix4d projection, Matrix4d view, Vector3d eyePosition);

		void createData(TerrainOctreeData* data);

		void checkNodeData();

	private:

		void checkSubdivisions(TerrainOctree* octree, Vector3d position);

	private:

		TerrainOctree* _octree;

		Scheduler* _scheduler;

		std::map<TerrainOctreeData*, TerrainOctreeDataTask*> _terrainDataTasks;
	};

}

#endif // !TERRAIN_H_

