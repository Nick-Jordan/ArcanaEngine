#ifndef VOXEL_TERRAIN_COMPONENT_H_
#define VOXEL_TERRAIN_COMPONENT_H_

#include "TerrainDefines.h"

#include "GeometryComponent.h"
#include "VoxelTerrainRenderProcedure.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API VoxelTerrainComponent : public GeometryComponent
	{
	public:

		VoxelTerrainComponent();

		virtual ~VoxelTerrainComponent();

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

	private:

		VoxelTerrainRenderProcedure* _terrainRenderProcedure;
	};

}

#endif // !VOXEL_TERRAIN_COMPONENT_H_

