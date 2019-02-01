#ifndef OBJECT_RENDERER_H_
#define OBJECT_RENDERER_H_

#include "GraphicsDefines.h"

//#include "DynamicVoxelConeTracingStage.h"
#include "DynamicPointShadowStage.h"
#include "DynamicDirectionalShadowStage.h"
#include "OpaqueEnvironmentStage.h"
#include "TransparentEnvironmentStage.h"
#include "OpaqueObjectStage.h"
#include "TransparentObjectStage.h"
#include "BackgroundSkyboxStage.h"
#include "DeferredLightingStage.h"
#include "PostProcessingStage.h"
#include "BloomCalculationStage.h"
#include "FinalHDRStage.h"

#include "Framebuffer.h"

#include <queue>

namespace Arcana
{
	class ARCANA_GRAPHICS_API ObjectRenderer
	{
	public:
		
		ObjectRenderer();

		~ObjectRenderer();

		void initialize();

		void finalize();

		//test cameraPosition
		void render(const Vector3d& cameraPosition);// Camera& camera, RenderTarget* renderTarget);

		void addMesh(const MeshRenderContext& context);

		void addLight(const RenderLight& light);

		//REMOVE THIS
		static void drawQuad();

		static void passRenderLight(uint32 index, Shader& shader, const RenderLight& light);

	private:
		
		struct
		{
			//DynamicVoxelConeTracingStage voxelConeTracing;
			DynamicPointShadowStage dynamicPointShadows;
			DynamicDirectionalShadowStage dynamicDirectionalShadows;
			OpaqueEnvironmentStage opaqueEnvironment;
			TransparentEnvironmentStage transparentEnvironment;
			OpaqueObjectStage opaqueObject;
			TransparentObjectStage transparentObject;
			BackgroundSkyboxStage backgroundSkybox;
			DeferredLightingStage deferredLightingStage;
			PostProcessingStage postProcessing;
			BloomCalculationStage bloomCalculation;
			//GraphicalUserInterfaceStage userInterface;
			FinalHDRStage finalHDR;

		} stages;

		uint32 _screenWidth;
		uint32 _screenHeight;

		Framebuffer* _gbuffer;

		Texture* _positionAO;
		Texture* _normalRoughness;
		Texture* _albedoSpecular;
		Texture* _emissiveMetallic;

		Texture* _hdrTexture;
		Texture* _hdrEmissiveTexture;

		Framebuffer* _hdrBuffer;
	};

}

#endif // !OBJECT_RENDERER_H_

