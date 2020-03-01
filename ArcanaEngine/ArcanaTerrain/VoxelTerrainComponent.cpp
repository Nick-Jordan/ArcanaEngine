#include "VoxelTerrainComponent.h"

namespace Arcana
{

	VoxelTerrainComponent::VoxelTerrainComponent(const Transform& transform) : _transform(transform)
	{
		initialize();
	}

	VoxelTerrainComponent::~VoxelTerrainComponent()
	{
		AE_RELEASE(_terrain);
	}

	void VoxelTerrainComponent::initialize()
	{
		_terrain = new VoxelTerrain();
		_terrain->reference();

		GeometryComponent::initialize();
	}

	void VoxelTerrainComponent::update(double elapsedTime)
	{
		//_terrainRenderProcedure->updateTerrain();
	}

	bool VoxelTerrainComponent::createRenderProcedure()
	{
		setTransform(_transform);

		_terrainRenderProcedure = new OctreeVoxelTerrainRenderProcedure(_terrain);
		_terrainRenderProcedure->reference();

		_renderProcedure = _terrainRenderProcedure;

		return true;
	}

}
