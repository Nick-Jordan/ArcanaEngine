#include "PostProcessingStage.h"

#include "TextureAttachment.h"
#include "GlobalShaders.h"
#include "ObjectRenderer.h"

#include "PostProcessor.h"

namespace Arcana
{

	PostProcessingStage::PostProcessingStage() : RenderStage("PostProcessingStage")
	{
	}


	PostProcessingStage::~PostProcessingStage()
	{
	}

	void PostProcessingStage::initialize()
	{
		_initialTexture = nullptr;

		uint32 screenWidth = 1920;
		uint32 screenHeight = 1080;

		Texture::Parameters params;
		params.setMagFilter(TextureFilter::Nearest);

		_framebufferTextures[0] = Texture::create2D(Texture::RGBA, screenWidth, screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);
		_framebufferTextures[1] = Texture::create2D(Texture::RGBA, screenWidth, screenHeight, Texture::RGBA32F, Texture::Float, nullptr, params);

		_framebuffers[0] = new Framebuffer("post_processing_stage_framebuffer_0");
		_framebuffers[1] = new Framebuffer("post_processing_stage_framebuffer_1");

		_framebuffers[0]->addAttachment(new TextureAttachment("framebuffer_texture_0", _framebufferTextures[0]));
		_framebuffers[1]->addAttachment(new TextureAttachment("framebuffer_texture_1", _framebufferTextures[1]));


		_finalShader = *GlobalShaders::get(GlobalShaders::TexturedQuad);

		for (uint32 i = 0; i < NUM_POST_PROCESS_EFFECTS; i++)
		{
			PostProcessEffect* effect = PostProcessor::Effects[i];
			if (effect)
			{
				effect->initialize();
			}
		}
	}

	void PostProcessingStage::finalize()
	{
		AE_DELETE(_framebuffers[0]);
		AE_DELETE(_framebuffers[1]);

		_initialTexture = nullptr;
	}

	void PostProcessingStage::render()
	{
		if (_initialTexture)

		{
			bool firstPass = true;
			bool framebuffer = false;

			for (auto i = PostProcessor::EffectQueue.begin(); i != PostProcessor::EffectQueue.end(); i++)
			{
				PostProcessEffect* effect = *i;

				if (!effect || !effect->isEnabled())
					continue;

				Framebuffer* prev = _framebuffers[framebuffer]->bind();

				effect->apply(firstPass ? _initialTexture : _framebufferTextures[!framebuffer]);

				framebuffer = !framebuffer;
				firstPass = false;

				Framebuffer::bind(prev);
			}

			_finalShader.bind();

			int32 unit = firstPass ? _initialTexture->bind() : _framebufferTextures[!framebuffer]->bind();
			_finalShader.getUniform("u_Texture").setValue(unit);
			ObjectRenderer::drawQuad();

			_finalShader.unbind();
		}

		_initialTexture = nullptr;
	}

	void PostProcessingStage::useInitialTexture(Texture* texture)
	{
		_initialTexture = texture;
	}
}
