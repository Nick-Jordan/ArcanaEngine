#include "ObjectRenderer.h"

#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"
#include "LightType.h"

#include "MeshIndexComponent.h"
#include "PostProcessor.h"

#include "Profiler.h"

namespace Arcana
{

	ObjectRenderer::ObjectRenderer()
	{

	}

	ObjectRenderer::~ObjectRenderer()
	{

	}

	void ObjectRenderer::initialize()
	{
		stages.dynamicPointShadows.initialize();
		stages.dynamicDirectionalShadows.initialize();
		stages.opaqueEnvironment.initialize();
		stages.transparentEnvironment.initialize();
		stages.opaqueObject.initialize();
		stages.transparentObject.initialize();
		stages.backgroundSkybox.initialize();
		stages.deferredLightingStage.initialize();
		stages.deferredDecalStage.initialize();
		stages.postProcessing.initialize();
		stages.bloomCalculation.initialize();
		stages.userInterface.initialize();

		_gbuffer = new Framebuffer("object_renderer_gbuffer");
		_hdrBuffer = new Framebuffer("object_renderer_hdrbuffer");

		_screenWidth = 1920;
		_screenHeight = 1080;

		Texture::Parameters params;
		params.setMagFilter(TextureFilter::Nearest);

		_positionAO = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_normalRoughness = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_albedoSpecular = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_emissiveMetallic = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_lightData = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_hdrTexture = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_hdrEmissiveTexture = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_depthMap = Texture::create2D(Texture::Depth, _screenWidth, _screenHeight, Texture::DEPTH_COMPONENT32F, Texture::Float, nullptr, params);
		_hdrDepthMap = Texture::create2D(Texture::Depth, _screenWidth, _screenHeight, Texture::DEPTH_COMPONENT32F, Texture::Float, nullptr, params);
		_gbuffer->addAttachment(new TextureAttachment("position_ao", _positionAO));
		_gbuffer->addAttachment(new TextureAttachment("normal_roughness", _normalRoughness));
		_gbuffer->addAttachment(new TextureAttachment("albedo_specular", _albedoSpecular));
		_gbuffer->addAttachment(new TextureAttachment("emissive_metallic", _emissiveMetallic));
		_gbuffer->addAttachment(new TextureAttachment("light_data", _lightData));
		//_gbuffer->addAttachment(new DepthStencilAttachment("depth", DepthStencilAttachment::Depth, _screenWidth, _screenHeight));

		Framebuffer* prev = _gbuffer->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap->getId(), 0);
		Framebuffer::bind(prev);

		_hdrBuffer->addAttachment(new TextureAttachment("hdr_texture", _hdrTexture));
		_hdrBuffer->addAttachment(new TextureAttachment("hdr_emissive_texture", _hdrEmissiveTexture));

		prev = _hdrBuffer->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _hdrDepthMap->getId(), 0);
		Framebuffer::bind(prev);
		//_hdrBuffer->addAttachment(new DepthStencilAttachment("depth", DepthStencilAttachment::Depth, _screenWidth, _screenHeight));
	}

	void ObjectRenderer::finalize()
	{
		AE_DELETE(_gbuffer);
		AE_DELETE(_hdrBuffer);
		AE_DELETE(_positionAO);
		AE_DELETE(_normalRoughness);
		AE_DELETE(_albedoSpecular);
		AE_DELETE(_emissiveMetallic);
		AE_DELETE(_lightData);
		AE_DELETE(_hdrTexture);
		AE_DELETE(_hdrEmissiveTexture);
		AE_DELETE(_depthMap);
		AE_DELETE(_hdrDepthMap);

		stages.dynamicPointShadows.finalize();
		stages.dynamicDirectionalShadows.finalize();
		stages.opaqueEnvironment.finalize();
		stages.transparentEnvironment.finalize();
		stages.opaqueObject.finalize();
		stages.transparentObject.finalize();
		stages.backgroundSkybox.finalize();
		stages.deferredLightingStage.finalize();
		stages.deferredDecalStage.finalize();
		stages.postProcessing.finalize();
		stages.bloomCalculation.finalize();
		stages.userInterface.finalize();
	}

	void ObjectRenderer::render(const RenderData& data)
	{
		//PROFILE("Object Renderer");

		//Voxel cone tracing for indirect lighting

		//Directional light dynamic shadow
		//{
		//	PROFILE("Dynamic Directional Shadows");
			stages.dynamicDirectionalShadows.render(data);
		//}
		//{
		//	PROFILE("Dynamic Point Shadows");
			stages.dynamicPointShadows.render(data);
		//}

		//render opaque objects into gbuffer----------------------------
		Framebuffer* prev = _gbuffer->bind();

		GLenum buffersG[5] = { Framebuffer::Color0, Framebuffer::Color1, Framebuffer::Color2, Framebuffer::Color3, Framebuffer::Color4 };
		_gbuffer->setDrawBuffers(buffersG, 5);

		glViewport(0, 0, _screenWidth, _screenHeight);//FIX  1280, 720
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //replace this clear
		//{
		//	PROFILE("Opaque Environment");
			stages.opaqueEnvironment.render(data);
		//}
		//{
		//	PROFILE("Opaque Object");
			stages.opaqueObject.render(data);
		//}

		Framebuffer::bind(prev);

		//deferred screen-space decals------------------------------

		prev = _gbuffer->bind();
		stages.deferredDecalStage.useTexture("u_DepthSampler", _depthMap);
		stages.deferredDecalStage.useTexture("u_AlbedoSpecular", _albedoSpecular);
		stages.deferredDecalStage.useTexture("u_NormalRoughness", _normalRoughness);
		stages.deferredDecalStage.useTexture("u_EmissiveMetallic", _emissiveMetallic);
		stages.deferredDecalStage.useTexture("u_PositionAO", _positionAO);
		stages.deferredDecalStage.useTexture("u_LightData", _lightData);
		stages.deferredDecalStage.render(data);
		Framebuffer::bind(prev);

		//lighting pass into hdr framebuffer------------------------------
		prev = _hdrBuffer->bind();

		GLenum buffersHdr[2] = { Framebuffer::Color0, Framebuffer::Color1 };
		_hdrBuffer->setDrawBuffers(buffersHdr, 2);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //replace this clear
		stages.deferredLightingStage.useGBufferTexture("u_PositionAO", _positionAO);
		stages.deferredLightingStage.useGBufferTexture("u_NormalRoughness", _normalRoughness);
		stages.deferredLightingStage.useGBufferTexture("u_AlbedoSpecular", _albedoSpecular);
		stages.deferredLightingStage.useGBufferTexture("u_EmissiveMetallic", _emissiveMetallic);
		stages.deferredLightingStage.useGBufferTexture("u_LightData", _lightData);
		stages.deferredLightingStage.shadow = stages.dynamicDirectionalShadows.shadow;
		stages.deferredLightingStage.passPointShadows(stages.dynamicPointShadows);
		{
			PROFILE("Deferred Lighting");
			stages.deferredLightingStage.render(data);
		}

		Framebuffer::bind(prev);

		//copy depth buffer to hdr framebuffer-------------------------------
		prev = _gbuffer->bind(Framebuffer::TargetReadFramebuffer);
		_hdrBuffer->bind(Framebuffer::TargetDrawFramebuffer);
		glBlitFramebuffer(0, 0, _screenWidth, _screenHeight, 0, 0, _screenWidth, _screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Framebuffer::bind(prev);

		//forward render transparent environment/objects---------------------
		prev = _hdrBuffer->bind();
		{
			PROFILE("Transparent Environment");
			stages.transparentEnvironment.render(data);
		}
		{
			PROFILE("Transparent Object");
			stages.transparentObject.render(data);
		}

		//render background/skybox------------------------------------------
		stages.backgroundSkybox.render(data);

		Framebuffer::bind(prev);

		//calculate bloom---------------------------------------------------
		stages.bloomCalculation.useEmissiveTexture(_hdrEmissiveTexture);
		{
			PROFILE("Bloom Calculation");
			stages.bloomCalculation.render(data);
		}

		//post processing
		{
			PROFILE("Post Processing");
			stages.postProcessing.useInitialTexture(_hdrTexture);
			//temp
			PostProcessor::get("EmissiveHDRComposite")->_clearedExtraTextures.push_back(stages.bloomCalculation.getEmissiveColorBuffer());
			stages.postProcessing.render(data);
		}

		//gui
		{
			PROFILE("User Interface");
			stages.userInterface.render(data);
		}

		stages.dynamicDirectionalShadows.clearProcedures();
		stages.dynamicPointShadows.clearProcedures();
		stages.opaqueEnvironment.clearProcedures();
		stages.opaqueObject.clearProcedures();
		stages.transparentEnvironment.clearProcedures();
		stages.transparentObject.clearProcedures();
		stages.backgroundSkybox.clearProcedures();
		stages.userInterface.clearProcedures();
		stages.deferredDecalStage.clearProcedures();

		stages.dynamicDirectionalShadows.clearLights();
		stages.dynamicPointShadows.clearLights();
		stages.deferredLightingStage.clearLights();
	}

	void ObjectRenderer::addProcedure(RenderProcedure* procedure)
	{
		if (procedure && procedure->isValidProcedure())
		{
			GlobalObjectID stage(procedure->Properties.RendererStage);

			if (stage == stages.opaqueEnvironment.getId())
			{
				stages.opaqueEnvironment.addProcedure(procedure);
			}
			else if (stage == stages.opaqueObject.getId())
			{
				stages.opaqueObject.addProcedure(procedure);
			}
			else if (stage == stages.transparentEnvironment.getId())
			{
				stages.transparentEnvironment.addProcedure(procedure);
			}
			else if (stage == stages.transparentObject.getId())
			{
				stages.transparentObject.addProcedure(procedure);
			}
			else if (stage == stages.backgroundSkybox.getId())
			{
				stages.backgroundSkybox.addProcedure(procedure);
			}
			else if (stage == stages.userInterface.getId())
			{
				stages.userInterface.addProcedure(procedure);
			}
			else if (stage == stages.deferredDecalStage.getId())
			{
				stages.deferredDecalStage.addProcedure(procedure);
			}
			//else if (stage == stages.bloomCalculation.getId())
			//{
			//	stages.bloomCalculation.addMesh(context);
			//}

			if (procedure->Properties.LightProperties.CastsDynamicShadow)
			{
				stages.dynamicDirectionalShadows.addProcedure(procedure);
				stages.dynamicPointShadows.addProcedure(procedure);
			}
		}
	}

	void ObjectRenderer::addLight(const RenderLight& light)
	{
		stages.deferredLightingStage.addLight(light);

		if (light.lightProperties.CastsDynamicShadow)
		{
			if (light.type == LightType::Directional)
			{
				stages.dynamicDirectionalShadows.addLight(light);
			}
			else if (light.type == LightType::Point)
			{
				stages.dynamicPointShadows.addLight(light);
			}
		}

		
	}

	uint32 quadVAO = 0;
	uint32 quadVBO;
	void ObjectRenderer::drawQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {

				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				1.0f, -1.0f,  1.0f, 0.0f,
				1.0f,  1.0f,  1.0f, 1.0f
			};

			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void ObjectRenderer::passRenderLight(uint32 index, Shader& shader, const RenderLight& light)
	{
		if (index < 16)  //replace 16 with MAX_LIGHTS
		{
			std::string i = std::to_string(index);
			
			shader.getUniform("u_Lights[" + i + "].color").setValue(light.color);
			shader.getUniform("u_Lights[" + i + "].intensity").setValue(light.intensity);
			shader.getUniform("u_Lights[" + i + "].type").setValue(light.type);
			//test
			shader.getUniform("u_Lights[" + std::to_string(index) + "].mobility").setValue(light.mobility);

			if (light.type == Point || light.type == Spot)
			{
				shader.getUniform("u_Lights[" + i + "].position").setValue(light.position);
				shader.getUniform("u_Lights[" + i + "].constant").setValue(light.constant);
				shader.getUniform("u_Lights[" + i + "].linear").setValue(light.linear);
				shader.getUniform("u_Lights[" + i + "].quadratic").setValue(light.quadratic);
				shader.getUniform("u_Lights[" + i + "].sourceRadius").setValue(light.sourceRadius);
				//shader.getUniform("u_Lights[" + i + "].softSourceRadius").setValue(light.softSourceRadius);
			}
			if (light.type == Spot)
			{
				shader.getUniform("u_Lights[" + i + "].direction").setValue(light.direction);
				shader.getUniform("u_Lights[" + i + "].innerAngle").setValue(light.innerAngle);
				shader.getUniform("u_Lights[" + i + "].outerAngle").setValue(light.outerAngle);
			}
			if (light.type == Directional)
			{
				shader.getUniform("u_Lights[" + i + "].direction").setValue(light.direction);
			}
			if (light.type == Area)
			{
				shader.getUniform("u_Lights[" + i + "].direction").setValue(light.direction);
				shader.getUniform("u_Lights[" + i + "].sourceWidth").setValue(light.sourceWidth);
				shader.getUniform("u_Lights[" + i + "].sourceHeight").setValue(light.sourceHeight);
			}
		}
	}
}