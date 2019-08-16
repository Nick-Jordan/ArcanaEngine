#include "OpaqueObjectStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

	OpaqueObjectStage::OpaqueObjectStage() : RenderStage("OpaqueObjectStage")
	{
	}


	OpaqueObjectStage::~OpaqueObjectStage()
	{
	}

	void OpaqueObjectStage::initialize()
	{
		_renderState.setBlendEnabled(false);
	}

	void OpaqueObjectStage::finalize()
	{

	}

	void OpaqueObjectStage::render()
	{
		_renderState.bind();

		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			ObjectRenderer::drawMeshContext(context);
		}

		_renderState.unbind();
	}
}
