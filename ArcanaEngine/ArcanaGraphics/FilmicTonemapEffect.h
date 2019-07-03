#ifndef FILMIC_TONEMAP_EFFECT_H_
#define FILMIC_TONEMAP_EFFECT_H_

#include "GraphicsDefines.h"

#include "PostProcessEffect.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API FilmicTonemapEffect : public PostProcessEffect
	{
	public:

		FilmicTonemapEffect();

		~FilmicTonemapEffect();

		virtual void apply(Texture* texture) override;

	private:

		float _exposure;
	};
}

#endif // !FILMIC_TONEMAP_EFFECT_H_

