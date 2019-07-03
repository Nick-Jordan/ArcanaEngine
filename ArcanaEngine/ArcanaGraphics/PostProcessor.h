#ifndef POST_PROCESSOR_H_
#define POST_PROCESSOR_H_

#include "GraphicsDefines.h"

#include "PostProcessEffect.h"
#include "PostProcessQueue.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API PostProcessor
	{
		friend class GraphicsModule;
		friend class PostProcessEffect;
		friend class PostProcessingStage;

	public:

		static void registerEffect(PostProcessEffect* effect);

		static PostProcessEffect* get(const std::string& name);

		static void registerDefaultEffects();

		static void buildQueue(const PostProcessQueue& queue);

	private:
		
		static PostProcessEffect* __blur;
		static PostProcessEffect* __colorGrading;
		static PostProcessEffect* __colorInversion;
		static PostProcessEffect* __edgeDetection;
		static PostProcessEffect* __emissiveHDRComposite;
		static PostProcessEffect* __filmicTonemapper;
		static PostProcessEffect* __FXAA;
		static PostProcessEffect* __grayscale;
		static PostProcessEffect* __nightVision;
		static PostProcessEffect* __sharpen;
		static PostProcessEffect* __vignette;

		static std::map<std::string, PostProcessEffect*> __registeredEffects;
		static std::vector<PostProcessEffect*> __effectQueue;

	private:

		static void initialize();

		static void finalize();
	};
}

#define EFFECT(x) Arcana::PostProcessor::get(x)

#endif // !POST_PROCESSOR_H_

