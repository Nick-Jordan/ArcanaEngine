#include "FilmicTonemapEffect.h"

namespace Arcana
{
	FilmicTonemapEffect::FilmicTonemapEffect() : PostProcessEffect("FilmicTonemap", "resources/arcana/shaders/effects/filmic_tonemapper_frag.glsl"), 
		_exposure(0.7f)
	{

	}

	FilmicTonemapEffect::~FilmicTonemapEffect()
	{
	}

	void FilmicTonemapEffect::apply(Texture* texture)
	{
		_shader->getUniform("u_Exposure").setValue(_exposure);
	}
}