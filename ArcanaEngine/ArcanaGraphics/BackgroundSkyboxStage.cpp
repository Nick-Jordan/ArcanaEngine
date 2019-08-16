#include "BackgroundSkyboxStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

	BackgroundSkyboxStage::BackgroundSkyboxStage() : RenderStage("BackgroundSkyboxStage")
	{
	}


	BackgroundSkyboxStage::~BackgroundSkyboxStage()
	{
	}

	void BackgroundSkyboxStage::initialize()
	{
		_renderState.setDepthTestEnabled(true);
		_renderState.setDepthFunction(RenderState::DepthFunction::LEqual);
	}

	void BackgroundSkyboxStage::finalize()
	{

	}

	void BackgroundSkyboxStage::render()
	{
		_renderState.bind();

		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			ObjectRenderer::drawMeshContext(context, false);
		}

		_renderState.unbind();
	}
}
