#include "DynamicDirectionalShadowStage.h"

#include "TextureAttachment.h"
#include "MeshIndexComponent.h"
#include "DepthStencilAttachment.h"
#include "ObjectRenderer.h"
#include "GlobalShaders.h"

namespace Arcana
{
	const uint32 DynamicDirectionalShadowStage::SHADOW_WIDTH = 1024;
	const uint32 DynamicDirectionalShadowStage::SHADOW_HEIGHT = 1024;


	DynamicDirectionalShadowStage::DynamicDirectionalShadowStage() : RenderStage("DynamicDirectionalShadowStage")
	{
	}


	DynamicDirectionalShadowStage::~DynamicDirectionalShadowStage()
	{
	}

	void DynamicDirectionalShadowStage::initialize()
	{
		_depthShader = *GlobalShaders::get(GlobalShaders::FTLShadowDepthMapping);

		_depthFramebuffer = new Framebuffer("dyanmic_directional_shadow_stage_depth_fbo");

		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Nearest);
		params.setMagFilter(TextureFilter::Nearest);
		params.setWrapS(TextureWrap::ClampBorder);
		params.setWrapT(TextureWrap::ClampBorder);
		params.setBorderf(1.0, 1.0, 1.0, 1.0);
		shadow.depthMap = Texture::create2D(Texture::Depth, SHADOW_WIDTH, SHADOW_HEIGHT, Texture::DEPTH_COMPONENT32F, Texture::Float, nullptr, params);

		//_depthFramebuffer->addAttachment(new TextureAttachment("depthMap", shadow.depthMap));

		Framebuffer* prev = _depthFramebuffer->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow.depthMap->getId(), 0);
		Framebuffer::bind(prev);

		//_depthFramebuffer->setDrawBuffer(Framebuffer::Color0);
		//prev = _depthFramebuffer->setReadBuffer(Framebuffer::None);
		//Framebuffer::bind(prev);
	}

	void DynamicDirectionalShadowStage::finalize()
	{
		AE_DELETE(_depthFramebuffer);
		AE_DELETE(shadow.depthMap);
	}

	void DynamicDirectionalShadowStage::render()
	{
		if (Lights.size() == 0)
			return;

		Framebuffer* prev = _depthFramebuffer->bind();

		_depthFramebuffer->setDrawBuffer(Framebuffer::None);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);//FIX
		glClear(GL_DEPTH_BUFFER_BIT); //replace this clear

		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);

		_depthShader.bind();

		//get direction from light
		Vector3f direction = Lights.size() > 0 ? Lights[0].position : Vector3f::zero();//Vector3f(-2.0f, 4.0f, -1.0f);
		shadow.position = direction;

		const float nearPlane = 1.0f, farPlane = 15.0f;
		Matrix4f lightProjection = Matrix4f::createOrthographic(20.0, 20.0, nearPlane, farPlane);//ortho(-10.0, 10.0, -10.0, 10.0, nearPlane, farPlane);
		Matrix4f lightView = Matrix4f::createLookAt(direction, Vector3f::zero(), Vector3f::unitY());
		shadow.lightSpaceMatrix = lightView * lightProjection;

		_depthShader.getUniform("u_LightSpaceMatrix").setValue(shadow.lightSpaceMatrix);

		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			if (context.isValid())
			{
				context.mesh->getVertexBuffer()->bind();

				uint32 componentCount = context.mesh->getNumIndexComponents();

				Mesh::InstanceProperties instanceProperties = context.mesh->getInstanceProperties();

				if (componentCount == 0)
				{
					_depthShader.getUniform("u_ModelMatrix").setValue(context.transform.getMatrix().cast<float>());

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
				}
				else
				{
					for (uint32 c = 0; c < componentCount; c++)
					{
						MeshIndexComponent* component = context.mesh->getIndexComponent(c);

						_depthShader.getUniform("u_ModelMatrix").setValue(context.transform.getMatrix().cast<float>());

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
					}					
				}

				context.mesh->getVertexBuffer()->unbind();
				context.renderProperties.renderState.unbind();
			}
		}

		_depthShader.unbind();

		glCullFace(GL_BACK);

		Framebuffer::bind(prev);
	}
}