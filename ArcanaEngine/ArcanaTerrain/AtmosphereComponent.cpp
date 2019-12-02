#include "AtmosphereComponent.h"

#include "Terrain.h"

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
		_atmosphereRenderProcedure = new AtmosphereRenderProcedure();
		_atmosphereRenderProcedure->reference();

		_renderProcedure = _atmosphereRenderProcedure;

		return true;
	}

}