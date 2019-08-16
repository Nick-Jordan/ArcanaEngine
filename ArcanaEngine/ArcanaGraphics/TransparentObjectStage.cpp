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
		_renderState.setBlendEnabled(true);
		_renderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		_renderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);
	}

	void TransparentObjectStage::finalize()
	{

	}

	void TransparentObjectStage::render()
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
