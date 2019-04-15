#ifndef FASTER_THAN_LIGHT_H_
#define FASTER_THAN_LIGHT_H_

#include "FTLDefines.h"

#include "World.h"
#include "StaticMeshComponent.h"
#include "BaseLightComponent.h"

#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "RectLightComponent.h"

#include "RayTracer.h"

#ifdef BUILD_LIGHTING
#define FASTER_THAN_LIGHT(world) { \
Arcana::FTL::LightProcessor processor(world); \
processor.run(); \
} do { } while(0)
#else
#define FASTER_THAN_LIGHT(world) { \
Arcana::FTL::LightProcessor processor(world); \
processor.read(); \
} do { } while(0)
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

			void run();

			void read();

			static Mesh* DebugMesh;

		private:

			void run3DRaytracer();

			void read3DRaytraceData();

			void runPhotonMapping();

			void raytrace(uint8* data, Vector3i size, const AxisAlignedBoundingBoxf& bounds, uint32 components);

			void addGeometry(StaticMeshComponent* geometry, bool dynamic = false);
			
			void addLight(BaseLightComponent* light);

			void generateTriangleList(std::vector<Triangle>& triangles);

		private:

			static void setColorFloat(uint8* data, Vector3d position, Vector3i size, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components = 3);

			static void addColorFloat(uint8* data, Vector3d position, Vector3i size, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components = 3);

			static void setColor(uint8* data, Vector3i position, Vector3i size, const Color& color, uint32 components = 3);

			static void setColor(uint8* data, uint8 x, uint8 y, uint8 z, uint8 size, const Color& color, uint32 components = 3);

			static void setRayColor(uint8* data, const Ray& ray, Vector3i size, uint32 partitions, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components = 3);

			static void addRayColor(uint8* data, const Ray& ray, Vector3i size, uint32 partitions, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components = 3);

			static Color getColor(uint8* data, Vector3i position, Vector3i size, uint32 components = 3);

			static LinearColor getLinearColor(uint8* data, Vector3i position, Vector3i size, uint32 components = 3);

			static uint8* blur(uint8* data, Vector3i size, Vector3i blur, uint32 components = 3);

		private:

			Array<StaticMeshComponent*> _geometry;

			Array<StaticMeshComponent*> _dynamicGeometry;

			Array<DirectionalLightComponent*> _directionalLights;
			Array<PointLightComponent*> _pointLights;
			Array<SpotLightComponent*> _spotLights;
			//Array<RectLightComponent*> _rectLights;
			//sky lights
		};
	}
}

#endif // !FASTER_THAN_LIGHT_H_
