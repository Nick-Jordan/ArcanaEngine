#include "DeferredDecalStage.h"

namespace Arcana
{
	DeferredDecalStage::DeferredDecalStage() : RenderStage("DeferredDecalStage")
	{

	}

	DeferredDecalStage::~DeferredDecalStage()
	{

	}

	void DeferredDecalStage::initialize()
	{													  //temp
		_textures.clear();
	}

	void DeferredDecalStage::finalize()
	{

	}
	
	void DeferredDecalStage::render(const RenderData& data)
	{
		for (auto i = Procedures.createConstIterator(); i; i++)
		{
			RenderProcedure* procedure = *i;

			if (procedure && procedure->isValidProcedure())
			{
				procedure->View = data.View;
				procedure->Projection = data.Projection;
				procedure->EyePosition = data.EyePosition;
				procedure->Properties.RenderState.setBlendEnabled(false);

				for (auto i = _textures.begin(); i != _textures.end(); i++)
				{
					RenderProcedure::UniformParameter sampler;
					sampler.name = (*i).key;
					sampler.value.type = Uniform::Value::Int32;
					sampler.value.i = (*i).value ? (*i).value->bind() : 0;
					procedure->Uniforms.push_back(sampler);
				}

				procedure->render();
			}
		}

		_textures.clear();
	}


	void DeferredDecalStage::useTexture(std::string s, Texture* texture)
	{
		_textures.push_back(MakePair(s, texture));
	}
}