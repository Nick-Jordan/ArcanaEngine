#include "VoxelTerrainComponent.h"

namespace Arcana
{

	VoxelTerrainComponent::VoxelTerrainComponent()
	{
		initialize();
	}

	VoxelTerrainComponent::~VoxelTerrainComponent()
	{
	}

	void VoxelTerrainComponent::initialize()
	{
		GeometryComponent::initialize();
	}

	void VoxelTerrainComponent::update(double elapsedTime)
	{
		
	}

	bool VoxelTerrainComponent::createRenderProcedure()
	{
		_terrainRenderProcedure = new VoxelTerrainRenderProcedure();
		_terrainRenderProcedure->reference();

		_renderProcedure = _terrainRenderProcedure;

		return true;
	}

}
