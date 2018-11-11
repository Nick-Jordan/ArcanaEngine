#include "GeometryComponent.h"

//test
#include "MeshRenderProcedure.h"

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

		//test
		createRenderProcedure();
	}

	bool GeometryComponent::hasRenderProcedure() const
	{
		return _renderProcedure != nullptr;
	}

	bool GeometryComponent::createRenderProcedure()
	{
		//test

		_renderProcedure = new MeshRenderProcedure();
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		return true;
	}

	void GeometryComponent::render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection)
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

				_renderProcedure->updateRenderData(update);

				//_renderProcedure->markDirty(false);
			}

			_renderProcedure->getRenderData()->render(renderer);
		}
	}
}
