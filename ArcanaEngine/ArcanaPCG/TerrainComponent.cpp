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

	bool TerrainComponent::createRenderProcedure()
	{
		LOGF(Warning, CoreEngine, "createRenderProcedure TerrainComponent");

		_renderProcedure = new TerrainRenderProcedure(_terrain);
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		LOGF(Warning, CoreEngine, "finished createRenderProcedure TerrainComponent");

		return true;
	}

}
