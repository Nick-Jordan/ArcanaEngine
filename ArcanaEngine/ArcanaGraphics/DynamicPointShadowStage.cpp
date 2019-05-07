#include "DynamicPointShadowStage.h"

#include "TextureAttachment.h"
#include "MeshIndexComponent.h"
#include "DepthStencilAttachment.h"
#include "ObjectRenderer.h"
#include "GlobalShaders.h"

namespace Arcana
{
	const uint32 DynamicPointShadowStage::SHADOW_WIDTH = 1024;
	const uint32 DynamicPointShadowStage::SHADOW_HEIGHT = 1024;


	DynamicPointShadowStage::DynamicPointShadowStage() : RenderStage("DynamicPointShadowStage")
	{
	}


	DynamicPointShadowStage::~DynamicPointShadowStage()
	{
	}

	void DynamicPointShadowStage::initialize()
	{
		_depthShader = *GlobalShaders::get(GlobalShaders::FTLPointShadowDepthMapping);

		_depthFramebuffer = new Framebuffer("dyanmic_point_shadow_stage_depth_fbo");

		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Nearest);
		params.setMagFilter(TextureFilter::Nearest);
		params.setWrapS(TextureWrap::ClampToEdge);
		params.setWrapT(TextureWrap::ClampToEdge);
		params.setWrapR(TextureWrap::ClampToEdge);
		shadow.depthMap = Texture::createCube(Texture::Depth, SHADOW_WIDTH, SHADOW_HEIGHT, Texture::DEPTH_COMPONENT32F, Texture::Float, nullptr, params);

		//_depthFramebuffer->addAttachment(new TextureAttachment("depthMap", shadow.depthMap));

		Framebuffer* prev = _depthFramebuffer->bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow.depthMap->getId(), 0);
		Framebuffer::bind(prev);

		//_depthFramebuffer->setDrawBuffer(Framebuffer::Color0);
		//prev = _depthFramebuffer->setReadBuffer(Framebuffer::None);
		//Framebuffer::bind(prev);
	}

	void DynamicPointShadowStage::finalize()
	{
		AE_DELETE(_depthFramebuffer);
		AE_DELETE(shadow.depthMap);
	}

	void DynamicPointShadowStage::render()
	{
		if (Lights.size() == 0)
			return;

		Framebuffer* prev = _depthFramebuffer->bind();

		_depthFramebuffer->setDrawBuffer(Framebuffer::None);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);//FIX
		glClear(GL_DEPTH_BUFFER_BIT); //replace this clear

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		_depthShader.bind();

		//get direction from light
		Vector3f position = Lights[0].position;//Vector3f(-2.0f, 4.0f, -1.0f);
		shadow.position = position;

		float nearPlane = 0.01f;
		float farPlane = 25.0f;
		Matrix4f shadowProj = Matrix4f::createPerspective(90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, nearPlane, farPlane);
		shadowProj.set(3, 3, 0.0f);
		std::vector<Matrix4f> shadowTransforms;
		shadowTransforms.push_back(Matrix4f::createLookAt(position, position + Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, -1.0f, 0.0f)) * shadowProj);
		shadowTransforms.push_back(Matrix4f::createLookAt(position, position + Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(0.0f, -1.0f, 0.0f)) * shadowProj);
		shadowTransforms.push_back(Matrix4f::createLookAt(position, position + Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)) * shadowProj);
		shadowTransforms.push_back(Matrix4f::createLookAt(position, position + Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f)) * shadowProj);
		shadowTransforms.push_back(Matrix4f::createLookAt(position, position + Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, -1.0f, 0.0f)) * shadowProj);
		shadowTransforms.push_back(Matrix4f::createLookAt(position, position + Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, -1.0f, 0.0f)) * shadowProj);

		for (uint32 i = 0; i < 6; ++i)
		{
			_depthShader.getUniform("u_ShadowMatrices[" + std::to_string(i) + "]").setValue(shadowTransforms[i]);
		}
		_depthShader.getUniform("u_FarPlane").setValue(farPlane);
		_depthShader.getUniform("u_LightPosition").setValue(position);

		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			if (context.isValid())
			{
				context.renderProperties.renderState.bind();

				context.callback.executeIfBound();

				if (context.hasMesh())
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
				}

				context.renderProperties.renderState.unbind();
			}
		}

		_depthShader.unbind();

		Framebuffer::bind(prev);
	}
}
