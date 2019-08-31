#include "TerrainComponent.h"

#include "TerrainRenderProcedure.h"

namespace Arcana
{

	TerrainComponent::TerrainComponent(const Terrain::Parameters& params, const Transform& transform) : _parameters(params), _transform(transform)
	{
		initialize();
	}


	TerrainComponent::~TerrainComponent()
	{
		AE_RELEASE(_terrain);
	}

	void TerrainComponent::initialize()
	{
		_terrain = new Terrain(_parameters);
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
		setTransform(_transform);

		_terrainRenderProcedure = new TerrainRenderProcedure(_terrain, "resources/terrain/planet_vert.glsl", "resources/terrain/planet_frag.glsl");
		_terrainRenderProcedure->reference();

		_renderProcedure = _terrainRenderProcedure;

		return true;
	}

}
