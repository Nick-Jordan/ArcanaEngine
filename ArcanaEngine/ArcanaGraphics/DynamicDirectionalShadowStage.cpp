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

		_numDirectionalShadows = 0;
	}

	void DynamicDirectionalShadowStage::finalize()
	{
		for (auto i = _shadows.begin(); i != _shadows.end(); i++)
		{
			AE_DELETE((*i).second.depthMap);
		}
		for (auto i = _depthFramebuffers.begin(); i != _depthFramebuffers.end(); i++)
		{
			AE_DELETE((*i).second);
		}
	}

	void DynamicDirectionalShadowStage::render(const RenderData& data)
	{
		if (Lights.size() == 0)
		{
			_numDirectionalShadows = 0;
			return;
		}

		_numDirectionalShadows = Lights.size();

		for (int32 s = 0; s < _numDirectionalShadows; s++)
		{
			UUID id = Lights[s].id;

			Framebuffer* fb = nullptr;

			auto update = _shadowUpdate.find(id);
			if (update == _shadowUpdate.end())
			{
				fb = createDepthFramebuffer(id);
			}
			else
			{
				if (!(*update).second)
				{
					continue;
				}

				fb = _depthFramebuffers[id];
			}

			_shadowUpdate[id] = false;

			DirectionalShadow& shadow = _shadows[id];

			Framebuffer* prev = fb->bind();

			fb->setDrawBuffer(Framebuffer::None);

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);//FIX
			glClear(GL_DEPTH_BUFFER_BIT); //replace this clear

			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_FRONT);

			_depthShader.bind();

			//get direction from light
			Vector3f direction = Lights[s].direction;//Vector3f(-2.0f, 4.0f, -1.0f);
			shadow.direction = direction;

			const float nearPlane = 1.0f, farPlane = 10000.0f;
			Matrix4f lightProjection = Matrix4f::createOrthographic(200.0f, 200.0f, nearPlane, farPlane);//ortho(-10.0, 10.0, -10.0, 10.0, nearPlane, farPlane);
			Matrix4f lightView = Matrix4f::createLookAt(direction * 20.0, Vector3f::zero(), Vector3f::normalize(Vector3f::cross(direction, Vector3f(0, 0, 1))));
			shadow.lightSpaceMatrix = lightView * lightProjection;

			_depthShader.getUniform("u_LightSpaceMatrix").setValue(shadow.lightSpaceMatrix);

			for (auto i = Procedures.createConstIterator(); i; i++)
			{
				RenderProcedure* procedure = *i;

				if (procedure && procedure->isValidProcedure())
				{
					_depthShader.getUniform("u_ModelMatrix").setValue(procedure->Transform.getMatrix().cast<float>());
					procedure->renderWithShader(_depthShader, false);
				}
			}

			_depthShader.unbind();

			glCullFace(GL_BACK);

			Framebuffer::bind(prev);
		}
	}

	const uint32 DynamicDirectionalShadowStage::getNumDirectionalShadows() const
	{
		return _numDirectionalShadows;
	}

	const std::map<UUID, DirectionalShadow>& DynamicDirectionalShadowStage::getDirectionalShadows() const
	{
		return _shadows;
	}

	Framebuffer* DynamicDirectionalShadowStage::createDepthFramebuffer(UUID id)
	{
		Framebuffer* depthFramebuffer = new Framebuffer("dyanmic_directional_shadow_stage_depth_fbo" + id.toString());

		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Nearest);
		params.setMagFilter(TextureFilter::Nearest);
		params.setWrapS(TextureWrap::ClampBorder);
		params.setWrapT(TextureWrap::ClampBorder);
		params.setBorderf(1.0, 1.0, 1.0, 1.0);
		DirectionalShadow s;
		s.depthMap = Texture::create2D(Texture::Depth, SHADOW_WIDTH, SHADOW_HEIGHT, Texture::DEPTH_COMPONENT32F, Texture::Float, nullptr, params);

		Framebuffer* prev = depthFramebuffer->bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, s.depthMap->getId(), 0);
		Framebuffer::bind(prev);

		_shadowUpdate[id] = true;
		_shadows[id] = s;
		_depthFramebuffers[id] = depthFramebuffer;

		return depthFramebuffer;
	}
}
