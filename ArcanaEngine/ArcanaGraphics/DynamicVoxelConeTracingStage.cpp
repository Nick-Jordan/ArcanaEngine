/*#include "DynamicVoxelConeTracingStage.h"

#include "ObjectRenderer.h"
#include "MeshIndexComponent.h"
#include "TextureAttachment.h"

namespace Arcana
{

	DynamicVoxelConeTracingStage::DynamicVoxelConeTracingStage() : RenderStage("DynamicVoxelConeTracingStage")
	{
	}


	DynamicVoxelConeTracingStage::~DynamicVoxelConeTracingStage()
	{
	}

	void DynamicVoxelConeTracingStage::initialize()
	{
		_voxelTextureSize = 64;

		_framebuffer = new Framebuffer("voxelization");

		_voxelizationShader.createProgram(Shader::Vertex, "resources/voxelization_vert.glsl");
		_voxelizationShader.createProgram(Shader::Geometry, "resources/voxelization_geom.glsl");
		Shader::Defines defines;
		defines.addDefine("MAX_LIGHTS", "16");
		_voxelizationShader.createProgram(Shader::Fragment, "resources/voxelization_frag.glsl", defines);

		Texture::Parameters params;
		params.setWrapS(TextureWrap::ClampBorder);
		params.setWrapT(TextureWrap::ClampBorder);
		params.setWrapR(TextureWrap::ClampBorder);
		params.setMinFilter(TextureFilter::LinearMipmapLinear);
		params.setMagFilter(TextureFilter::Nearest);
		_voxelTexture = Texture::create3D(Texture::RGBA, _voxelTextureSize, _voxelTextureSize, _voxelTextureSize,
			Texture::RGBA8, Texture::Float, nullptr, params, true);

		_framebufferTexture = Texture::create2D(Texture::RGBA, _voxelTextureSize, _voxelTextureSize, Texture::RGBA8, Texture::UnsignedByte, nullptr);

		_framebuffer->addAttachment(new TextureAttachment("texture", _framebufferTexture));

		glBindTexture(_voxelTexture->getType(), _voxelTexture->getId());
		glTextureStorage3D(_voxelTexture->getId(), 7, GL_RGBA8, _voxelTextureSize, _voxelTextureSize, _voxelTextureSize);
		glBindTexture(_voxelTexture->getType(), 0);

		_automaticallyVoxelize = true;
		_voxelizationSparsity = 1;
		_ticksSinceLastVoxelization = 0;
	}

	void DynamicVoxelConeTracingStage::finalize()
	{
		AE_DELETE(_framebuffer);
		AE_DELETE(_voxelTexture);
	}

	void DynamicVoxelConeTracingStage::render()
	{
		bool shouldVoxelize = _voxelizationQueued || (_automaticallyVoxelize && _voxelizationSparsity > 0 && ++_ticksSinceLastVoxelization >= _voxelizationSparsity);
		if (!shouldVoxelize)
		{
			return;
		}

		LOG(Info, CoreEngine, "Voxelization");
		LOGF(Info, CoreEngine, "Error: %d", glGetError());
		
		_ticksSinceLastVoxelization = 0;
		_voxelizationQueued = false;

		Framebuffer* prev = _framebuffer->bind();

		glBindTexture(GL_TEXTURE_3D, _voxelTexture->getId());
		GLfloat clearColor[4] = { 0, 0, 0, 0 };
		glClearTexImage(_voxelTexture->getId(), 0, GL_RGBA, GL_FLOAT, &clearColor);
		glBindTexture(GL_TEXTURE_3D, 0);

		_voxelizationShader.bind();

		glViewport(0, 0, _voxelTextureSize, _voxelTextureSize);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		uint32 unit = _voxelTexture->bind();
		_voxelizationShader.getUniform("voxelTexture").setValue(unit);
		glBindImageTexture(unit, _voxelTexture->getId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);



		uint32 i = 0;
		for (auto lightIter = Lights.createConstIterator(); lightIter; lightIter++)
		{
			ObjectRenderer::passRenderLight(i++, _voxelizationShader, *lightIter);
		}

		_voxelizationShader.getUniform("u_NumLights").setValue(Lights.size());

		// Render.
		renderObjects();
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		//if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
		glGenerateMipmap(GL_TEXTURE_3D);
		//regenerateMipmapQueued = false;
		//}
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		Framebuffer::bind(prev);
	}

	Texture* DynamicVoxelConeTracingStage::getVoxelTexture() const
	{
		return _voxelTexture;
	}

	void DynamicVoxelConeTracingStage::renderObjects()
	{
		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			if (context.isValid())
			{
				context.renderState.bind();

				context.mesh->getVertexBuffer()->bind();

				uint32 componentCount = context.mesh->getNumIndexComponents();

				if (componentCount == 0)
				{
					context.material->passMaterialAttributes(&_voxelizationShader);

					//Default Uniforms
					_voxelizationShader.getUniform("u_ProjectionMatrix").setValue(context.projectionMatrix.cast<float>());
					_voxelizationShader.getUniform("u_ViewMatrix").setValue(context.viewMatrix.cast<float>());
					_voxelizationShader.getUniform("u_ModelMatrix").setValue(context.transform.getMatrix().cast<float>());
					_voxelizationShader.getUniform("u_NormalMatrix").setValue(context.transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
					_voxelizationShader.getUniform("u_CameraPosition").setValue(context.eyePosition.cast<float>());

					for (uint32 j = 0; j < context.uniforms.size(); j++)
					{
						_voxelizationShader.getUniform(context.uniforms[j].name).setValue(context.uniforms[j].value);
					}

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glDrawArrays(context.mesh->getPrimitive(), 0, context.mesh->getNumVertices());
				}
				else
				{
					if (componentCount != context.material->getTechniqueCount())
					{
						LOG(Warning, CoreEngine, "Material technique count not equal to component count");
					}

					for (uint32 c = 0; c < componentCount; c++)
					{
						MeshIndexComponent* component = context.mesh->getIndexComponent(c);

						context.material->passMaterialAttributes(&_voxelizationShader);

						//Default Uniforms
						_voxelizationShader.getUniform("u_ProjectionMatrix").setValue(context.projectionMatrix.cast<float>());
						_voxelizationShader.getUniform("u_ViewMatrix").setValue(context.viewMatrix.cast<float>());
						_voxelizationShader.getUniform("u_ModelMatrix").setValue(context.transform.getMatrix().cast<float>());
						_voxelizationShader.getUniform("u_NormalMatrix").setValue(context.transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
						_voxelizationShader.getUniform("u_CameraPosition").setValue(context.eyePosition.cast<float>());

						component->getIndexBuffer()->bind();
						glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
						component->getIndexBuffer()->unbind();
					}
				}

				context.mesh->getVertexBuffer()->unbind();
				context.renderState.unbind();
			}
		}
	}
}*/
