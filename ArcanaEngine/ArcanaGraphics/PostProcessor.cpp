#include "PostProcessor.h"

#include "VignetteEffect.h"

namespace Arcana
{
	PostProcessEffect* PostProcessor::Blur = nullptr;
	PostProcessEffect* PostProcessor::ColorInversion = nullptr;
	PostProcessEffect* PostProcessor::EdgeDetection = nullptr;
	PostProcessEffect* PostProcessor::EmissiveHDRComposite = nullptr;
	PostProcessEffect* PostProcessor::FilmicTonemapper = nullptr;
	PostProcessEffect* PostProcessor::FXAA = nullptr;
	PostProcessEffect* PostProcessor::Grayscale = nullptr;
	PostProcessEffect* PostProcessor::NightVision = nullptr;
	PostProcessEffect* PostProcessor::Sharpen = nullptr;
	PostProcessEffect* PostProcessor::Vignette = nullptr;

	PostProcessEffect** PostProcessor::Effects = nullptr;
	std::vector<PostProcessEffect*> PostProcessor::EffectQueue;

	void PostProcessor::initialize()
	{
		Blur = new PostProcessEffect("resources/arcana/shaders/effects/blur_frag.glsl");
		ColorInversion = new PostProcessEffect("resources/arcana/shaders/effects/color_inversion_frag.glsl");
		EdgeDetection = new PostProcessEffect("resources/arcana/shaders/effects/edge_detection_frag.glsl");
		EmissiveHDRComposite = new PostProcessEffect("resources/arcana/shaders/effects/emissive_hdr_composite_frag.glsl");
		FilmicTonemapper = new PostProcessEffect("resources/arcana/shaders/effects/filmic_tonemapper_frag.glsl");
		FXAA = new PostProcessEffect("resources/arcana/shaders/effects/fxaa_frag.glsl");
		Grayscale = new PostProcessEffect("resources/arcana/shaders/effects/grayscale_frag.glsl");
		NightVision = new PostProcessEffect("resources/arcana/shaders/effects/night_vision_frag.glsl");
		Sharpen = new PostProcessEffect("resources/arcana/shaders/effects/sharpen_frag.glsl");
		Vignette = new VignetteEffect();

		//enable default effects
		EmissiveHDRComposite->enable();
		FilmicTonemapper->enable();
		FXAA->enable();

		static PostProcessEffect* effects[NUM_POST_PROCESS_EFFECTS] = 
		{
			Blur, ColorInversion, EdgeDetection, EmissiveHDRComposite,
			FilmicTonemapper, FXAA, Grayscale, NightVision, Sharpen,
			Vignette
		};

		Effects = effects;
	}

	void PostProcessor::finalize()
	{
		AE_DELETE(Blur);
		AE_DELETE(ColorInversion);
		AE_DELETE(EdgeDetection);
		AE_DELETE(EmissiveHDRComposite);
		AE_DELETE(FilmicTonemapper);
		AE_DELETE(FXAA);
		AE_DELETE(Grayscale);
		AE_DELETE(NightVision);
		AE_DELETE(Sharpen);
	}
}
