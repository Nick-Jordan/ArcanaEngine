#ifndef FTL_RAY_TRACER_H_
#define FTL_RAY_TRACER_H_

#include "FTLDefines.h"

#include "Types.h"
#include "Ray.h"
#include "PhotonMap.h"
#include "Triangle.h"
#include "PointLight.h"
#include <vector>

namespace Arcana
{
	namespace FTL
	{
		class ARCANA_FTL_API RayTracer
		{
		public:

			RayTracer(const std::vector<Triangle>& triangles, const std::vector<PointLight>& lights);/////MORE LIGHTS??????

			~RayTracer();

			static LinearColor raytrace(
				PhotonMap& photonMap,
				PhotonMap& photonMapCaustic,
				const Ray& ray,
				const std::vector<Triangle>& triangleList,
				int32 bounce,
				int32 maxLightBounces,
				float refractionIndex
			);

			static LinearColor irradiance(
				PhotonMap& photonMap,
				const Ray& ray,
				const std::vector<Triangle>& triangleList,
				int32 bounce,
				int32 maxLightBounces,
				float refractionIndex);

		public:

			static bool intersect(const Ray& ray, const std::vector<Triangle>& triangles, HitResult& hit, int32& id);

			static Vector3d randomDirection();

			static bool intersect(const Sphered& sphere, const Ray& ray);

		private:

			std::vector<Triangle> _triangles;
		};
	}
}

#endif // !FTL_RAY_TRACER_H_

