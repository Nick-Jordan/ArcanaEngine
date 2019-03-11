#include "ObjectRenderer.h"

#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"
#include "LightType.h"

#include "MeshIndexComponent.h"

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
		stages.postProcessing.initialize();
		stages.bloomCalculation.initialize();
		stages.finalHDR.initialize();


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

		stages.dynamicPointShadows.finalize();
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
		stages.dynamicPointShadows.render();

		//render opaque objects into gbuffer----------------------------
		Framebuffer* prev = _gbuffer->bind();

		GLenum buffersG[4] = { Framebuffer::Color0, Framebuffer::Color1, Framebuffer::Color2, Framebuffer::Color3 };
		_gbuffer->setDrawBuffers(buffersG, 4);

		glViewport(0, 0, _screenWidth, _screenHeight);//FIX  1280, 720
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
		stages.deferredLightingStage.shadowPoint = stages.dynamicPointShadows.shadow;
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
		stages.dynamicPointShadows.clearMeshes();
		stages.opaqueEnvironment.clearMeshes();
		stages.opaqueObject.clearMeshes();
		stages.transparentEnvironment.clearMeshes();
		stages.transparentObject.clearMeshes();
		stages.backgroundSkybox.clearMeshes();

		stages.dynamicDirectionalShadows.clearLights();
		stages.dynamicPointShadows.clearLights();
		stages.deferredLightingStage.clearLights();
	}

	void ObjectRenderer::addMesh(const MeshRenderContext& context)
	{
		if (context.isValid())
		{
			GlobalObjectID stage(context.renderProperties.rendererStage);

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

			if (context.renderProperties.lightProperties.CastsDynamicShadow)
			{
				stages.dynamicDirectionalShadows.addMesh(context);
				stages.dynamicPointShadows.addMesh(context);
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
			//LOGF(Info, CoreEngine, "Light[%d]: %f, %f, %f | %f, %f, %f", index, light.position.x, light.position.y, light.position.z, light.color.x, light.color.y, light.color.z);

			shader.getUniform("u_Lights[" + std::to_string(index) + "].position").setValue(light.position);
			shader.getUniform("u_Lights[" + std::to_string(index) + "].color").setValue(light.color);
			shader.getUniform("u_Lights[" + std::to_string(index) + "].type").setValue(light.type);
		}
	}

	void ObjectRenderer::drawMeshContext(MeshRenderContext& context)
	{
		context.callback.executeIfBound();

		if (context.hasMesh())
		{
			context.renderProperties.renderState.bind();
			context.mesh->getVertexBuffer()->bind();

			Mesh::InstanceProperties instanceProperties = context.mesh->getInstanceProperties();

			uint32 componentCount = context.mesh->getNumIndexComponents();

			if (componentCount == 0)
			{
				Technique* technique = context.material->getCurrentTechnique();
				if (technique)
				{
					context.material->bindMaterialTextures(technique);

					for (uint32 i = 0; i < technique->getPassCount(); i++)
					{
						Shader* pass = technique->getPass(i);
						if (pass)
						{
							pass->bind();

							context.material->passMaterialAttributes(pass, technique);

							//pass FTL results

							//Default Uniforms
							pass->getUniform("u_ProjectionMatrix").setValue(context.projectionMatrix.cast<float>());
							pass->getUniform("u_ViewMatrix").setValue(context.viewMatrix.cast<float>());
							pass->getUniform("u_ModelMatrix").setValue(context.transform.getMatrix().cast<float>());
							pass->getUniform("u_NormalMatrix").setValue(context.transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
							pass->getUniform("u_CameraPosition").setValue(context.eyePosition.cast<float>());

							for (uint32 j = 0; j < context.uniforms.size(); j++)
							{
								pass->getUniform(context.uniforms[j].name).setValue(context.uniforms[j].value);
							}

							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							if (instanceProperties.isInstanced())
							{
								context.mesh->getInstanceBuffer()->bind();
								glDrawArraysInstanced(context.mesh->getPrimitive(), 0, context.mesh->getNumVertices(), instanceProperties.getNumInstances());
								context.mesh->getInstanceBuffer()->unbind();
							}
							else
							{							
								glDrawArrays(context.mesh->getPrimitive(), 0, context.mesh->getNumVertices());
							}

							pass->unbind();
						}
					}
				}
			}
			else
			{
				if (componentCount != context.material->getTechniqueCount())
				{
					LOG(Warning, CoreEngine, "Material technique count not equal to component count");
				}

				for (uint32 c = 0; c < componentCount; c++)
				{
					Technique* technique = context.material->getTechnique(c);
					if (!technique)
					{
						technique = context.material->getCurrentTechnique();
					}

					if (technique)
					{
						context.material->bindMaterialTextures(technique);

						MeshIndexComponent* component = context.mesh->getIndexComponent(c);
						for (uint32 i = 0; i < technique->getPassCount(); i++)
						{
							Shader* pass = technique->getPass(i);
							if (pass)
							{
								pass->bind();

								context.material->passMaterialAttributes(pass, technique);

								//Default Uniforms
								pass->getUniform("u_ProjectionMatrix").setValue(context.projectionMatrix.cast<float>());
								pass->getUniform("u_ViewMatrix").setValue(context.viewMatrix.cast<float>());
								pass->getUniform("u_ModelMatrix").setValue(context.transform.getMatrix().cast<float>());
								pass->getUniform("u_NormalMatrix").setValue(context.transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
								pass->getUniform("u_CameraPosition").setValue(context.eyePosition.cast<float>());

								component->getIndexBuffer()->bind();
								if (instanceProperties.isInstanced())
								{
									context.mesh->getInstanceBuffer()->bind();
									glDrawElementsInstanced(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0, instanceProperties.getNumInstances());
									context.mesh->getInstanceBuffer()->unbind();
								}
								else
								{
									glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
								}
								component->getIndexBuffer()->unbind();
								pass->unbind();
							}
						}
					}
				}
			}

			context.mesh->getVertexBuffer()->unbind();
			context.renderProperties.renderState.unbind();
		}
	}
}