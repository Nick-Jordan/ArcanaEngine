#include "GraphicalUserInterfaceStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

	GraphicalUserInterfaceStage::GraphicalUserInterfaceStage() : RenderStage("GraphicalUserInterfaceStage")
	{
	}


	GraphicalUserInterfaceStage::~GraphicalUserInterfaceStage()
	{
	}

	void GraphicalUserInterfaceStage::initialize()
	{

	}

	void GraphicalUserInterfaceStage::finalize()
	{

	}

	void GraphicalUserInterfaceStage::render()
	{
		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			ObjectRenderer::drawMeshContext(context);
		}
	}
}
