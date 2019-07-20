#include "FilmicTonemapEffect.h"

namespace Arcana
{
	float FilmicTonemapEffect::Exposure = 0.7f;

	FilmicTonemapEffect::FilmicTonemapEffect() : PostProcessEffect("FilmicTonemap", "resources/arcana/shaders/effects/filmic_tonemapper_frag.glsl")
	{

	}

	FilmicTonemapEffect::~FilmicTonemapEffect()
	{
	}

	void FilmicTonemapEffect::apply(Texture* texture)
	{
		_shader->getUniform("u_Exposure").setValue(Exposure);
	}
}