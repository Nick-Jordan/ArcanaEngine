#include "ObjectRenderer.h"

#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"
#include "LightType.h"

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
		stages.dynamicDirectionalShadows.initialize();
		stages.opaqueEnvironment.initialize();
		stages.transparentEnvironment.initialize();
		stages.opaqueObject.initialize();
		stages.transparentObject.initialize();
		stages.backgroundSkybox.initialize();
		stages.deferredLightingStage.initialize();
		stages.postProcessing.initialize();
		stages.bloomCalculation.initialize();
		stages.finalHDR.initialize();


		_gbuffer = new Framebuffer("object_renderer_gbuffer");
		_hdrBuffer = new Framebuffer("object_renderer_hdrbuffer");

		_screenWidth = 1280;
		_screenHeight = 720;

		Texture::Parameters params;
		params.setMagFilter(TextureFilter::Nearest);

		_positionAO = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_normalRoughness = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_albedoSpecular = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_emissiveMetallic = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);

		_hdrTexture = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_hdrEmissiveTexture = Texture::create2D(Texture::RGBA, _screenWidth, _screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);

		_gbuffer->addAttachment(new TextureAttachment("position_ao", _positionAO));
		_gbuffer->addAttachment(new TextureAttachment("normal_roughness", _normalRoughness));
		_gbuffer->addAttachment(new TextureAttachment("albedo_specular", _albedoSpecular));
		_gbuffer->addAttachment(new TextureAttachment("emissive_metallic", _emissiveMetallic));
		_gbuffer->addAttachment(new DepthStencilAttachment("depth", DepthStencilAttachment::Depth, _screenWidth, _screenHeight));

		_hdrBuffer->addAttachment(new TextureAttachment("hdr_texture", _hdrTexture));
		_hdrBuffer->addAttachment(new TextureAttachment("hdr_emissive_texture", _hdrEmissiveTexture));
		_hdrBuffer->addAttachment(new DepthStencilAttachment("depth", DepthStencilAttachment::Depth, _screenWidth, _screenHeight));
	}

	void ObjectRenderer::finalize()
	{
		AE_DELETE(_gbuffer);
		AE_DELETE(_hdrBuffer);
		AE_DELETE(_positionAO);
		AE_DELETE(_normalRoughness);
		AE_DELETE(_albedoSpecular);
		AE_DELETE(_emissiveMetallic);
		AE_DELETE(_hdrTexture);
		AE_DELETE(_hdrEmissiveTexture);

		stages.dynamicDirectionalShadows.finalize();
		stages.opaqueEnvironment.finalize();
		stages.transparentEnvironment.finalize();
		stages.opaqueObject.finalize();
		stages.transparentObject.finalize();
		stages.backgroundSkybox.finalize();
		stages.deferredLightingStage.finalize();
		stages.postProcessing.finalize();
		stages.bloomCalculation.finalize();
		stages.finalHDR.finalize();
	}

	void ObjectRenderer::render(const Vector3d& cameraPosition)
	{
		//Voxel cone tracing for indirect lighting

		//Directional light dynamic shadow
		stages.dynamicDirectionalShadows.render();

		//render opaque objects into gbuffer----------------------------
		Framebuffer* prev = _gbuffer->bind();

		GLenum buffersG[4] = { Framebuffer::Color0, Framebuffer::Color1, Framebuffer::Color2, Framebuffer::Color3 };
		_gbuffer->setDrawBuffers(buffersG, 4);

		glViewport(0, 0, 1280, 720);//FIX
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //replace this clear
		stages.opaqueEnvironment.render();
		stages.opaqueObject.render();

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
		stages.deferredLightingStage.setCameraPosition(cameraPosition);
		stages.deferredLightingStage.shadow = stages.dynamicDirectionalShadows.shadow;
		stages.deferredLightingStage.render();

		Framebuffer::bind(prev);

		//copy depth buffer to hdr framebuffer-------------------------------
		prev = _gbuffer->bind(Framebuffer::TargetReadFramebuffer);
		_hdrBuffer->bind(Framebuffer::TargetDrawFramebuffer);
		glBlitFramebuffer(0, 0, _screenWidth, _screenHeight, 0, 0, _screenWidth, _screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Framebuffer::bind(prev);

		//forward render transparent environment/objects---------------------
		prev = _hdrBuffer->bind();
		stages.transparentEnvironment.render();
		stages.transparentObject.render();

		//render background/skybox------------------------------------------
		// glDepthFunc(GL_LEQUAL);  SET THIS!!!
		//stages.backgroundSkybox.render();

		Framebuffer::bind(prev);

		//calculate bloom---------------------------------------------------
		stages.bloomCalculation.useEmissiveTexture(_hdrEmissiveTexture);
		stages.bloomCalculation.render();

		//post processing

		//draw hdr texture onto screen (with tonemapping/exposure)----------
		stages.finalHDR.useHDRTexture(_hdrTexture);
		stages.finalHDR.useEmissiveTexture(stages.bloomCalculation.getEmissiveColorBuffer());
		stages.finalHDR.render();

		//gui

		stages.dynamicDirectionalShadows.clearMeshes();
		stages.opaqueEnvironment.clearMeshes();
		stages.opaqueObject.clearMeshes();
		stages.transparentEnvironment.clearMeshes();
		stages.transparentObject.clearMeshes();
		stages.backgroundSkybox.clearMeshes();

		stages.dynamicDirectionalShadows.clearLights();
		stages.deferredLightingStage.clearLights();
	}

	void ObjectRenderer::addMesh(const MeshRenderContext& context)
	{
		if (context.isValid())
		{
			GlobalObjectID stage(context.rendererStage);

			if (stage == stages.opaqueEnvironment.getId())
			{
				stages.opaqueEnvironment.addMesh(context);
			}
			else if (stage == stages.opaqueObject.getId())
			{
				stages.opaqueObject.addMesh(context);
			}
			else if (stage == stages.transparentEnvironment.getId())
			{
				stages.transparentEnvironment.addMesh(context);
			}
			else if (stage == stages.transparentObject.getId())
			{
				stages.transparentObject.addMesh(context);
			}
			else if (stage == stages.backgroundSkybox.getId())
			{
				stages.backgroundSkybox.addMesh(context);
			}
			//else if (stage == stages.bloomCalculation.getId())
			//{
			//	stages.bloomCalculation.addMesh(context);
			//}

			//check directional
			stages.dynamicDirectionalShadows.addMesh(context);
		}
	}

	void ObjectRenderer::addLight(const RenderLight& light)
	{
		stages.deferredLightingStage.addLight(light);
		LOGF(Info, CoreEngine, "type: %d, %d, %d, %f", light.type, LightType::Directional, LightType::Point, light.position.z);
		if (light.type == LightType::Directional)
		{
			stages.dynamicDirectionalShadows.addLight(light);
		}
	}

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
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
			//LOGF(Info, CoreEngine, "Light[%d]: %f, %f, %f | %f, %f, %f", index, light.position.x, light.position.y, light.position.z, light.color.x, light.color.y, light.color.z);

			shader.getUniform("u_Lights[" + std::to_string(index) + "].position")->setValue(light.position);
			shader.getUniform("u_Lights[" + std::to_string(index) + "].color")->setValue(light.color);
			shader.getUniform("u_Lights[" + std::to_string(index) + "].type")->setValue(light.type);
		}
	}
}