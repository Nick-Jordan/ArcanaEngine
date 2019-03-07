#include "TerrainComponent.h"

#include "TerrainRenderProcedure.h"

namespace Arcana
{

	TerrainComponent::TerrainComponent(const Terrain::Parameters& params, Transform* transform) : _parameters(params)
	{
		if (transform)
		{
			_transform = *transform;
			AE_RELEASE(transform);
		}
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

		_renderProcedure = new TerrainRenderProcedure(_terrain, _transform, "resources/terrain/new_terrain_vert.glsl", "resources/terrain/new_terrain_frag.glsl");
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		_terrainRenderProcedure = dynamic_cast<TerrainRenderProcedure*>(_renderProcedure);

		return true;
	}

}
