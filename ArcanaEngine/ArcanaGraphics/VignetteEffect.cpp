#include "VignetteEffect.h"

namespace Arcana
{
	VignetteEffect::VignetteEffect() 
		: PostProcessEffect("Vignette", "resources/arcana/shaders/effects/vignette_frag.glsl"), _vignetteTexture(nullptr)
	{

	}

	VignetteEffect::~VignetteEffect()
	{
		AE_DELETE(_vignetteTexture);
	}

	void VignetteEffect::apply(Texture* texture)
	{
		int32 unit = _vignetteTexture->bind();
		_shader->getUniform("u_VignetteTexture").setValue(unit);

		_shader->getUniform("u_VignetteScale").setValue(1.5f);
	}

	void VignetteEffect::initialize()
	{
		PostProcessEffect::initialize();

		if (!_vignetteTexture)
		{
			Image<uint8> image;
			image.init("resources/arcana/textures/effects/vignette.png");
			Texture::Parameters params;
			params.setMinFilter(TextureFilter::Linear);
			params.setMagFilter(TextureFilter::Linear);
			_vignetteTexture = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params);
		}
	}
}