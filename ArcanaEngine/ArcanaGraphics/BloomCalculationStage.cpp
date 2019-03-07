#include "BloomCalculationStage.h"

#include "ObjectRenderer.h"
#include "TextureAttachment.h"
#include "GlobalShaders.h"

namespace Arcana
{

	BloomCalculationStage::BloomCalculationStage() : RenderStage("BloomCalculationStage")
	{
	}


	BloomCalculationStage::~BloomCalculationStage()
	{
	}

	void BloomCalculationStage::initialize()
	{
		uint32 screenWidth = 1920;
		uint32 screenHeight = 1080;

		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Linear);
		params.setMagFilter(TextureFilter::Linear);
		_pingpongColorBuffer[0] = Texture::create2D(Texture::RGBA, screenWidth, screenHeight, Texture::RGBA16F, Texture::Float, nullptr, params);
		_pingpongColorBuffer[1] = Texture::create2D(Texture::RGBA, screenWidth, screenHeight, Texture::RGBA16F, Texture::Float, nullptr, params);
		_pingpongFBO[0] = new Framebuffer("bloom_calculation_pingpong_fbo0");
		_pingpongFBO[0]->addAttachment(new TextureAttachment("pingpong_colorbuffer0", _pingpongColorBuffer[0]));
		_pingpongFBO[1] = new Framebuffer("bloom_calculation_pingpong_fbo1");
		_pingpongFBO[1]->addAttachment(new TextureAttachment("pingpong_colorbuffer1", _pingpongColorBuffer[1]));
		_firstEmissiveTexture = nullptr;

		_blurShader = *GlobalShaders::get(GlobalShaders::GaussianBlur);
	}

	void BloomCalculationStage::finalize()
	{
		AE_DELETE(_pingpongFBO[0]);
		AE_DELETE(_pingpongFBO[1]);
		AE_DELETE(_pingpongColorBuffer[0]);
		AE_DELETE(_pingpongColorBuffer[1]);
		_firstEmissiveTexture = nullptr;
	}

	void BloomCalculationStage::render()
	{
		_horizontal = true;
		bool first_iteration = true;
		uint32 amount = 10;
		_blurShader.bind();
		for (uint32 i = 0; i < amount; i++)
		{
			Framebuffer* prev = _pingpongFBO[_horizontal]->bind();
			_blurShader.getUniform("horizontal").setValue(_horizontal);
			int32 unit = first_iteration ? _firstEmissiveTexture->bind() : _pingpongColorBuffer[!_horizontal]->bind();
			_blurShader.getUniform("u_EmissiveTexture").setValue(unit);

			ObjectRenderer::drawQuad();

			_horizontal = !_horizontal;
			if (first_iteration)
				first_iteration = false;

			Framebuffer::bind(prev);
		}
		_blurShader.unbind();

		_firstEmissiveTexture = nullptr;
	}

	Texture* BloomCalculationStage::getEmissiveColorBuffer() const
	{
		return _pingpongColorBuffer[!_horizontal];
	}

	void BloomCalculationStage::useEmissiveTexture(Texture* texture)
	{
		_firstEmissiveTexture = texture;
	}
}
