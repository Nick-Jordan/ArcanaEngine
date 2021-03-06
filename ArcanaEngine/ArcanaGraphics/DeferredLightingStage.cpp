#include "DeferredLightingStage.h"

#include "ObjectRenderer.h"
#include "GlobalShaders.h"

namespace Arcana
{

	DeferredLightingStage::DeferredLightingStage() : RenderStage("DeferredLightingStage")
	{
	}


	DeferredLightingStage::~DeferredLightingStage()
	{
	}

	void DeferredLightingStage::initialize()
	{
		_lightingShader = *GlobalShaders::get(GlobalShaders::DeferredLighting);
	}

	void DeferredLightingStage::finalize()
	{
		_gbufferTextures.clear();
	}

	void DeferredLightingStage::render(const RenderData& data)
	{
		_lightingShader.bind();

		std::vector< KeyValuePair<std::string, Texture*>>::iterator iter;
		for (iter = _gbufferTextures.begin(); iter != _gbufferTextures.end(); iter++)
		{
			int32 unit = (*iter).value->bind();//material?
			_lightingShader.getUniform((*iter).key).setValue(unit);
		}


		uint32 i = 0;
		for (auto lightIter = Lights.createConstIterator(); lightIter; lightIter++)
		{
			ObjectRenderer::passRenderLight(i++, _lightingShader, *lightIter);
		}
		_lightingShader.getUniform("u_NumLights").setValue(Lights.size());

		_lightingShader.getUniform("u_CameraPosition").setValue(data.EyePosition.cast<float>());

		ObjectRenderer::drawQuad();

		_lightingShader.unbind();

		_gbufferTextures.clear();
	}

	void DeferredLightingStage::useGBufferTexture(const std::string& samplerName, Texture* texture)
	{
		_gbufferTextures.push_back(MakePair(samplerName, texture));
	}

	void  DeferredLightingStage::passDirectionalShadows(DynamicDirectionalShadowStage& stage)
	{
		int32 index = 0;
		for (auto i = stage.getDirectionalShadows().begin(); i != stage.getDirectionalShadows().end(); i++)
		{
			passDirectionalShadow(index++, _lightingShader, (*i).second);
		}

		_lightingShader.getUniform("u_NumDirectionalShadows").setValue((int32)stage.getNumDirectionalShadows());
	}

	void  DeferredLightingStage::passPointShadows(DynamicPointShadowStage& stage)
	{
		int32 index = 0;
		for (auto i = stage.getPointShadows().begin(); i != stage.getPointShadows().end(); i++)
		{
			passPointShadow(index++, _lightingShader, (*i).second);
		}

		_lightingShader.getUniform("u_NumPointShadows").setValue((int32)stage.getNumPointShadows());
	}

	void DeferredLightingStage::passDirectionalShadow(uint32 index, Shader& shader, const DirectionalShadow& shadow)
	{
		if (index < 16)  //replace 16 with MAX_LIGHTS
		{
			if (shadow.depthMap)
			{
				int32 unit = shadow.depthMap->bind();
				shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].depthMap").setValue(unit);
			}
			shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].lightSpaceMatrix").setValue(shadow.lightSpaceMatrix);
			shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].direction").setValue(shadow.direction);
		}
	}

	void DeferredLightingStage::passPointShadow(uint32 index, Shader& shader, const PointShadow& shadow)
	{
		if (index < 16)  //replace 16 with MAX_LIGHTS
		{
			if (shadow.depthMap)
			{
				int32 unit = shadow.depthMap->bind();
				shader.getUniform("u_PointShadows[" + std::to_string(index) + "].depthMap").setValue(unit);
			}
			shader.getUniform("u_PointShadows[" + std::to_string(index) + "].position").setValue(shadow.position);
			shader.getUniform("u_PointShadows[" + std::to_string(index) + "].bias").setValue(shadow.bias);
		}
	}
}
