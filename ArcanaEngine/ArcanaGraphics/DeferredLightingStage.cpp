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

	void DeferredLightingStage::render()
	{
		_lightingShader.bind();

		std::vector< std::pair<std::string, Texture*>>::iterator iter;
		for (iter = _gbufferTextures.begin(); iter != _gbufferTextures.end(); iter++)
		{
			int32 unit = iter->second->bind();//material?
			_lightingShader.getUniform(iter->first).setValue(unit);
		}


		uint32 i = 0;
		for (auto lightIter = Lights.createConstIterator(); lightIter; lightIter++)
		{
			ObjectRenderer::passRenderLight(i++, _lightingShader, *lightIter);
		}
		_lightingShader.getUniform("u_NumLights").setValue(Lights.size());

		//directional shadows
		i = 0;
		//passDirectionalShadow(i++, _lightingShader, shadow);
		_lightingShader.getUniform("u_NumDirectionalShadows").setValue(0);//num

		//point shadows
		i = 0;
		passPointShadow(i++, _lightingShader, shadowPoint);
		_lightingShader.getUniform("u_NumPointShadows").setValue(1);//num


		_lightingShader.getUniform("u_CameraPosition").setValue(_cameraPosition.cast<float>());

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
				shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].depthMap").setValue(unit);
			}
			shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].lightSpaceMatrix").setValue(shadow.lightSpaceMatrix);
			shader.getUniform("u_DirectionalShadows[" + std::to_string(index) + "].position").setValue(shadow.position);
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
			shader.getUniform("u_PointShadows[" + std::to_string(index) + "].lightSpaceMatrix").setValue(shadow.lightSpaceMatrix);
			shader.getUniform("u_PointShadows[" + std::to_string(index) + "].position").setValue(shadow.position);
		}
	}
}
