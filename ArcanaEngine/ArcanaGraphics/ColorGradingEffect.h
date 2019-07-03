#ifndef COLOR_GRADING_EFFECT_H_
#define COLOR_GRADING_EFFECT_H_

#include "GraphicsDefines.h"

#include "PostProcessEffect.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API ColorGradingEffect : public PostProcessEffect
	{
	public:

		ColorGradingEffect();

		~ColorGradingEffect();

		virtual void apply(Texture* texture) override;

		virtual void initialize() override;

	private:

		Texture* _colorGradingLUT;
	};
}

#endif // !COLOR_GRADING_EFFECT_H_

