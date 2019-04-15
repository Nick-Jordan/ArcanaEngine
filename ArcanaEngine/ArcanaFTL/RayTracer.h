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

			void raytrace(uint32 raysPerLight, uint32 numLightBounces, std::vector<Ray>& rays);

		private:

			void raytrace(Ray& ray, uint32 depth, uint32 numLightBounces, std::vector<Ray>& rays); //caustic map?????  photon map??????

		public:

			static bool intersect(Ray& ray, const std::vector<Triangle>& triangles, double& t, int32& id);

			static bool intersect(const Ray& ray, const std::vector<Triangle>& triangles, double& t, int32& id);

			static Vector3d randomDirection();

			static bool intersect(const Sphered& sphere, const Ray& ray);

		private:

			std::vector<Triangle> _triangles;
		};
	}
}

#endif // !FTL_RAY_TRACER_H_

