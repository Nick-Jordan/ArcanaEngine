#include "AtmosphereComponent.h"

namespace Arcana
{

	AtmosphereComponent::AtmosphereComponent()
	{
		initialize();
	}

	AtmosphereComponent::~AtmosphereComponent()
	{
	}

	void AtmosphereComponent::initialize()
	{
		GeometryComponent::initialize();

		Terrain::createAtmosphereTextures();
	}

	void AtmosphereComponent::update(double elapsedTime)
	{
	}

	bool AtmosphereComponent::createRenderProcedure()
	{
		_renderProcedure = new AtmosphereRenderProcedure(Transform());
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		_atmosphereRenderProcedure = dynamic_cast<AtmosphereRenderProcedure*>(_renderProcedure);

		return true;
	}

}
