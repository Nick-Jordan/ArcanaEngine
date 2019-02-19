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

	}

	void OpaqueObjectStage::finalize()
	{

	}

	void OpaqueObjectStage::render()
	{
		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			ObjectRenderer::drawMeshContext(context);
		}
	}
}
