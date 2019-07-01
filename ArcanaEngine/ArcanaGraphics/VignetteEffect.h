#ifndef VIGNETTE_EFFECT_H_
#define VIGNETTE_EFFECT_H_

#include "GraphicsDefines.h"

#include "PostProcessEffect.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API VignetteEffect : public PostProcessEffect
	{
	public:

		VignetteEffect();

		~VignetteEffect();

		virtual void apply(Texture* texture) override;

		virtual void initialize() override;


	private:

		Texture* _vignetteTexture;
	};
}

#endif // !VIGNETTE_EFFECT_H_

