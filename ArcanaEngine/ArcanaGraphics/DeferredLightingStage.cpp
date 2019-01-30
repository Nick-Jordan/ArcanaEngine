#include "DeferredLightingStage.h"

#include "ObjectRenderer.h"

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
		_lightingShader.createProgram(Shader::Vertex, "resources/quad_vert.glsl");
		Shader::Defines defines;
		defines.addDefine("MAX_LIGHTS", "16");
		_lightingShader.createProgram(Shader::Fragment, "resources/deferred_stage_frag.glsl", defines);
	}

	void DeferredLightingStage::finalize()
	{
		_gbufferTextures.clear();
	}

	void DeferredLightingStage::render()
	{
		_lightingShader.bind();

		std::vector< std::pair<std::string, Texture*>>::iterator iter;
		for (iter = _gbufferTextures.begin(); iter != _gbufferTextures.end(); iter++)
		{
			int32 unit = iter->second->bind();//material?
			_lightingShader.getUniform(iter->first)->setValue(unit);
		}


		uint32 i = 0;
		for (auto lightIter = Lights.createConstIterator(); lightIter; lightIter++)
		{
			ObjectRenderer::passRenderLight(i++, _lightingShader, *lightIter);
		}
		_lightingShader.getUniform("u_NumLights")->setValue(Lights.size());

		i = 0;
		passDirectionalShadow(i++, _lightingShader, shadow);
		_lightingShader.getUniform("u_NumDirectionalShadows")->setValue(1);


		_lightingShader.getUniform("u_CameraPosition")->setValue(_cameraPosition.cast<float>());

		ObjectRenderer::drawQuad();

		_lightingShader.unbind();

		_gbufferTextures.clear();
	}

	void DeferredLightingStage::setCameraPosition(const Vector3d& cameraPosition)
	{
		_cameraPosition = cameraPosition;
	}

	void DeferredLightingStage::useGBufferTexture(const std::string& samplerName, Texture* texture)
	{
		_gbufferTextures.push_back(std::make_pair(samplerName, texture));
	}

	void DeferredLightingStage::passDirectionalShadow(uint32 index, Shader& shader, const DirectionalShadow& shadow)
	{
		if (index < 16)  //replace 16 with MAX_LIGHTS
		{
			if (shadow.depthMap)
			{
				int32 unit = shadow.depthMap->bind();
				shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].depthMap")->setValue(unit);
			}
			shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].lightSpaceMatrix")->setValue(shadow.lightSpaceMatrix);
			shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].position")->setValue(shadow.position);
		}
	}
}
