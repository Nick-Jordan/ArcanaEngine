#include "ColorGradingEffect.h"

namespace Arcana
{
	ColorGradingEffect::ColorGradingEffect()
		: PostProcessEffect("ColorGrading", "resources/arcana/shaders/effects/color_grading_frag.glsl"), _colorGradingLUT(nullptr)
	{

	}

	ColorGradingEffect::~ColorGradingEffect()
	{
		AE_DELETE(_colorGradingLUT);
	}

	void ColorGradingEffect::apply(Texture* texture)
	{
		int32 unit = _colorGradingLUT->bind();
		_shader->getUniform("u_ColorGradingLUT").setValue(unit);
	}

	void ColorGradingEffect::initialize()
	{
		PostProcessEffect::initialize();

		if (!_colorGradingLUT)
		{
			Image<uint8> image;
			image.init("resources/arcana/textures/effects/color_grading_LUT_2.png");
			Texture::Parameters params;
			params.setMinFilter(TextureFilter::Linear);
			params.setMagFilter(TextureFilter::Linear);
			_colorGradingLUT = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params);
		}
	}
}