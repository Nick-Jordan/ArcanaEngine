#include "TerrainComponent.h"

#include "TerrainRenderProcedure.h"

namespace Arcana
{

	TerrainComponent::TerrainComponent()
	{
		initialize();
	}


	TerrainComponent::~TerrainComponent()
	{
		AE_RELEASE(_terrain);
	}

	void TerrainComponent::initialize()
	{
		_terrain = new Terrain();
		_terrain->reference();

		GeometryComponent::initialize();
	}

	void TerrainComponent::update(double elapsedTime)
	{
		if (_terrainRenderProcedure)
		{
			//_terrainRenderProcedure->updateTerrain();
		}
	}

	bool TerrainComponent::createRenderProcedure()
	{
		LOGF(Warning, CoreEngine, "createRenderProcedure TerrainComponent");

		_renderProcedure = new TerrainRenderProcedure(_terrain);
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		_terrainRenderProcedure = dynamic_cast<TerrainRenderProcedure*>(_renderProcedure);

		LOGF(Warning, CoreEngine, "finished createRenderProcedure TerrainComponent");

		return true;
	}

}
