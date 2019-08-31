/*#include "AtmosphereComponent.h"

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

	void AtmosphereComponent::updateRenderData(Matrix4d view, Matrix4d projection, Vector3d eyePosition)
	{
		RenderDataUpdate update;
		update.view = view;
		update.projection = projection;
		update.eyePosition = eyePosition;
		//copy light properties;
		//update.lightProperties = _lightProperties;
		update.transform.set(getWorldTransform());

		_atmosphereRenderProcedure->updateRenderData(update);
	}

	bool AtmosphereComponent::createRenderProcedure()
	{
		_renderProcedure = new AtmosphereRenderProcedure(Transform());
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		_atmosphereRenderProcedure = dynamic_cast<AtmosphereRenderProcedure*>(_renderProcedure);

		return true;
	}

}*/