#ifndef POST_PROCESSOR_H_
#define POST_PROCESSOR_H_

#include "GraphicsDefines.h"

#include "PostProcessEffect.h"

#define NUM_POST_PROCESS_EFFECTS 10

namespace Arcana
{
	class ARCANA_GRAPHICS_API PostProcessor
	{
		friend class GraphicsModule;
		friend class PostProcessEffect;

	public:
		
		static PostProcessEffect* Blur;
		static PostProcessEffect* ColorInversion;
		static PostProcessEffect* EdgeDetection;
		static PostProcessEffect* EmissiveHDRComposite;
		static PostProcessEffect* FilmicTonemapper;
		static PostProcessEffect* FXAA;
		static PostProcessEffect* Grayscale;
		static PostProcessEffect* NightVision;
		static PostProcessEffect* Sharpen;
		static PostProcessEffect* Vignette;

		static PostProcessEffect** Effects;
		static std::vector<PostProcessEffect*> EffectQueue;

	private:

		static void initialize();

		static void finalize();
	};
}

#endif // !POST_PROCESSOR_H_

