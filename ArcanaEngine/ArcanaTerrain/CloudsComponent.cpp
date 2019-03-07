#include "CloudsComponent.h"

#include "TerrainRenderProcedure.h"

namespace Arcana
{

	CloudsComponent::CloudsComponent(const Terrain::Parameters& params, Transform* transform) : _parameters(params)
	{
		if (transform)
		{
			_transform = *transform;
			AE_RELEASE(transform);
		}
		initialize();
	}


	CloudsComponent::~CloudsComponent()
	{
		AE_RELEASE(_terrain);
	}

	void CloudsComponent::initialize()
	{
		_terrain = new Terrain(_parameters);
		_terrain->reference();

		GeometryComponent::initialize();
	}

	void CloudsComponent::update(double elapsedTime)
	{
		if (_terrainRenderProcedure)
		{
			//_terrainRenderProcedure->updateTerrain();
		}
	}

	bool CloudsComponent::createRenderProcedure()
	{
		setTransform(_transform);

		_renderProcedure = new TerrainRenderProcedure(_terrain, _transform, "resources/terrain/clouds/instanced_clouds_vert.glsl", "resources/terrain/clouds/instanced_clouds_frag.glsl");
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		_terrainRenderProcedure = dynamic_cast<TerrainRenderProcedure*>(_renderProcedure);

		return true;
	}

}
