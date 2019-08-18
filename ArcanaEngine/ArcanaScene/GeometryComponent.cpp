#include "GeometryComponent.h"

namespace Arcana
{
	GeometryComponent::GeometryComponent() : _renderProcedure(nullptr)
	{
		initialize();
	}

	GeometryComponent::~GeometryComponent()
	{
		if (_renderProcedure)
		{
			AE_RELEASE(_renderProcedure);
		}
	}

	void GeometryComponent::initialize()
	{
		SceneComponent::initialize();

		createRenderProcedure();
	}

	bool GeometryComponent::hasRenderProcedure() const
	{
		return _renderProcedure != nullptr;
	}

	bool GeometryComponent::createRenderProcedure()
	{
		return false;
	}

	void GeometryComponent::updateRenderProcedure()
	{

	}

	RenderProcedure* GeometryComponent::getRenderProcedure() const
	{
		return _renderProcedure;
	}

	void GeometryComponent::render(ObjectRenderer& renderer)
	{
		AE_ASSERT(hasRenderProcedure());

		updateRenderProcedure();
		_renderProcedure->Transform.set(getWorldTransform());
		renderer.addProcedure(_renderProcedure);
	}
}
