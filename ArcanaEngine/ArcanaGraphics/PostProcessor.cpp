#include "PostProcessor.h"

#include "ColorGradingEffect.h"
#include "FilmicTonemapEffect.h"
#include "VignetteEffect.h"
#include "GraphicsLoggers.h"

namespace Arcana
{
	PostProcessEffect* PostProcessor::__blur = nullptr;
	PostProcessEffect* PostProcessor::__colorGrading = nullptr;
	PostProcessEffect* PostProcessor::__colorInversion = nullptr;
	PostProcessEffect* PostProcessor::__edgeDetection = nullptr;
	PostProcessEffect* PostProcessor::__emissiveHDRComposite = nullptr;
	PostProcessEffect* PostProcessor::__filmicTonemapper = nullptr;
	PostProcessEffect* PostProcessor::__FXAA = nullptr;
	PostProcessEffect* PostProcessor::__grayscale = nullptr;
	PostProcessEffect* PostProcessor::__nightVision = nullptr;
	PostProcessEffect* PostProcessor::__sharpen = nullptr;
	PostProcessEffect* PostProcessor::__vignette = nullptr;

	std::map<std::string, PostProcessEffect*> PostProcessor::__registeredEffects;
	std::vector<PostProcessEffect*> PostProcessor::__effectQueue;

	void PostProcessor::registerEffect(PostProcessEffect* effect)
	{
		if (effect)
		{
			__registeredEffects.emplace(effect->getName(), effect);
		}
	}

	PostProcessEffect* PostProcessor::get(const std::string& name)
	{
		auto iter = __registeredEffects.find(name);

		if (iter != __registeredEffects.end())
		{
			(*iter).second->initialize();

			return (*iter).second;
		}

		LOGF(Warning, PostProcessorLog, "Unable to find post processing effect with name: \'%s\'", name.c_str());
		return nullptr;
	}

	void PostProcessor::registerDefaultEffects()
	{
		registerEffect(__blur);
		registerEffect(__colorGrading);
		registerEffect(__colorInversion);
		registerEffect(__edgeDetection);
		registerEffect(__emissiveHDRComposite);
		registerEffect(__filmicTonemapper);
		registerEffect(__FXAA);
		registerEffect(__grayscale);
		registerEffect(__nightVision);
		registerEffect(__sharpen);
		registerEffect(__vignette);
	}

	void PostProcessor::buildQueue(const PostProcessQueue& queue)
	{
		queue.process(PostProcessor::__effectQueue);
	}

	void PostProcessor::initialize()
	{
		__blur = new PostProcessEffect("Blur", "resources/arcana/shaders/effects/blur_frag.glsl");
		__colorGrading = new ColorGradingEffect();
		__colorInversion = new PostProcessEffect("ColorInversion", "resources/arcana/shaders/effects/color_inversion_frag.glsl");
		__edgeDetection = new PostProcessEffect("EdgeDetection", "resources/arcana/shaders/effects/edge_detection_frag.glsl");
		__emissiveHDRComposite = new PostProcessEffect("EmissiveHDRComposite", "resources/arcana/shaders/effects/emissive_hdr_composite_frag.glsl");
		__filmicTonemapper = new FilmicTonemapEffect();
		__FXAA = new PostProcessEffect("FXAA", "resources/arcana/shaders/effects/fxaa_frag.glsl");
		__grayscale = new PostProcessEffect("Grayscale", "resources/arcana/shaders/effects/grayscale_frag.glsl");
		__nightVision = new PostProcessEffect("NightVision", "resources/arcana/shaders/effects/night_vision_frag.glsl");
		__sharpen = new PostProcessEffect("Sharpen", "resources/arcana/shaders/effects/sharpen_frag.glsl");
		__vignette = new VignetteEffect();
	}

	void PostProcessor::finalize()
	{
		AE_DELETE(__blur);
		AE_DELETE(__colorGrading);
		AE_DELETE(__colorInversion);
		AE_DELETE(__edgeDetection);
		AE_DELETE(__emissiveHDRComposite);
		AE_DELETE(__filmicTonemapper);
		AE_DELETE(__FXAA);
		AE_DELETE(__grayscale);
		AE_DELETE(__nightVision);
		AE_DELETE(__sharpen);
		AE_DELETE(__vignette);
	}
}
