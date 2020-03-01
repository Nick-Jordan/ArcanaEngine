#ifndef VOXEL_TERRAIN_COMPONENT_H_
#define VOXEL_TERRAIN_COMPONENT_H_

#include "TerrainDefines.h"

#include "GeometryComponent.h"
#include "OctreeVoxelTerrainRenderProcedure.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API VoxelTerrainComponent : public GeometryComponent
	{
	public:

		VoxelTerrainComponent(const Transform& transform = Transform());

		virtual ~VoxelTerrainComponent();

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

	private:

		OctreeVoxelTerrainRenderProcedure* _terrainRenderProcedure;
		Transform _transform;

		VoxelTerrain* _terrain;
	};

}

#endif // !VOXEL_TERRAIN_COMPONENT_H_

