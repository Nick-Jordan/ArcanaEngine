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

	void BackgroundSkyboxStage::render(const RenderData& data)
	{
		for (auto i = Procedures.createConstIterator(); i; i++)
		{
			RenderProcedure* procedure = *i;

			if (procedure && procedure->isValidProcedure())
			{
				procedure->View = data.View;
				procedure->Projection = data.Projection;
				procedure->EyePosition = data.EyePosition;
				procedure->Properties.RenderState.setDepthTestEnabled(true);
				procedure->Properties.RenderState.setDepthFunction(RenderState::DepthFunction::LEqual);

				procedure->render();
			}
		}
	}
}
