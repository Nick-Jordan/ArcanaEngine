#ifndef FASTER_THAN_LIGHT_H_
#define FASTER_THAN_LIGHT_H_

#include "FTLDefines.h"

#include "World.h"
#include "GeometryComponent.h"
#include "BaseLightComponent.h"

#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "RectLightComponent.h"

#ifdef BUILD_LIGHTING
#define FASTER_THAN_LIGHT(world) Arcana::FTL::LightProcessor processor(world)
#else
#define FASTER_THAN_LIGHT(world) do { } while(0)
#endif

namespace Arcana
{
	namespace FTL
	{
		class ARCANA_FTL_API LightProcessor
		{
		public:

			LightProcessor(World* world);

			~LightProcessor();

		private:

			void addGeometry(GeometryComponent* geometry);
			
			void addLight(BaseLightComponent* light);

			void createDirectionalShadowMaps();

			Texture* createDirectionalLightDepthMap(Shader& depthShader, DirectionalLightComponent* directionalLight, uint32 width, uint32 height);

			Texture* createDirectionalShadowMap(Shader& shadowMapShader, DirectionalLightComponent* directionalLight, Texture* depthMap, GeometryComponent* geometry, uint32 width, uint32 height);

			void renderGeometry(Shader& shader);

			void renderGeometry(Shader& shader, GeometryComponent* component);

		private:

			Array<GeometryComponent*> _geometry;

			Array<DirectionalLightComponent*> _directionalLights;
			Array<PointLightComponent*> _pointLights;
			Array<SpotLightComponent*> _spotLights;
			//Array<RectLightComponent*> _rectLights;
			//sky lights

			ObjectRenderer _renderer;
		};
	}
}

#endif // !FASTER_THAN_LIGHT_H_
