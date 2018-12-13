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

	void GeometryComponent::render(ObjectRenderer& renderer, Matrix4d view, Matrix4d projection, Vector3d eyePosition)
	{
		AE_ASSERT(hasRenderProcedure());
		
		if (_renderProcedure->isValidProcedure())
		{
			AE_ASSERT(_renderProcedure->getRenderData());

			if (_renderProcedure->isDirty())
			{
				RenderDataUpdate update;
				update.view = view;
				update.projection = projection;
				update.eyePosition = eyePosition;

				_renderProcedure->updateRenderData(update);

				//_renderProcedure->markDirty(false);
			}

			_renderProcedure->getRenderData()->render(renderer);
		}
	}
}
