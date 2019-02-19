#include "TransparentObjectStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

	TransparentObjectStage::TransparentObjectStage() : RenderStage("TransparentObjectStage")
	{
	}


	TransparentObjectStage::~TransparentObjectStage()
	{
	}

	void TransparentObjectStage::initialize()
	{

	}

	void TransparentObjectStage::finalize()
	{

	}

	void TransparentObjectStage::render()
	{
		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			ObjectRenderer::drawMeshContext(context);
		}
	}
}
